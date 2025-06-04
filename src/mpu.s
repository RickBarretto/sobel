@ --------------------------------------------------------------------
@ Biblioteca  MPU para Laplace
@ --------------------------------------------------------------------

.syntax unified               @ Configuração da sintaxe
.arch armv7-a                 @ Usando arquitetura ARMv7-A
.thumb                        @ Usando modo Thumb para instruções
.text

@ --------------------------------------------------------------------
@ delay_microsecond(void) -> void
@ --------------------------------------------------------------------
@ Gera um atraso de 1 microssegundo usando nanosleep
@ --------------------------------------------------------------------
.thumb_func
.type	delay_microsecond, %function
delay_microsecond:
    @ Preserva registradores e configura frame
    push    {r7, lr}              @ Salva r7 e link register
    sub     sp, sp, #8            @ Reserva 8 bytes na pilha
    add     r7, sp, #0            @ Configura frame pointer
    
    @ Configura struct timespec para nanosleep 
    @ (1000 nanosegundos = 1 microssegundo)
    mov     r3, #0                @ timespec.tv_sec = 0
    str     r3, [r7, #0]
    mov     r3, #1000             @ timespec.tv_nsec = 1000
    str     r3, [r7, #4]
    
    @ Chama nanosleep com os parâmetros apropriados
    mov     r3, r7                @ Primeiro parâmetro: ponteiro para timespec
    mov     r0, r3
    mov     r1, #0                @ Segundo parâmetro: NULL
    bl      nanosleep
    
    @ Restaura stack e retorna
    add     r7, r7, #8
    mov     sp, r7
    pop     {r7, pc}              @ Restaura registradores e retorna

@ --------------------------------------------------------------------
@ Strings constantes para mensagens de erro do `mpu_new_connection`
@ --------------------------------------------------------------------
.section    .rodata
.align    2
str_dev_mem:
    .ascii    "/dev/mem\000"
    .align    2
str_open_error:
    .ascii    "open(/dev/mem)\000"
    .align    2
str_mmap_error:
    .ascii    "mmap\000"
    .align    2
str_munmap_error:
    .ascii    "munmap\000"

    .text
    .align    2

@ --------------------------------------------------------------------
@ mpu_new_connection(void) -> Connection
@ --------------------------------------------------------------------
@ Inicializa uma conexão para a MPU mapeando memória.
@
@ @param  r0: ponteiro para a estrutura de conexão
@ @return r0: Connection (struct)
@ --------------------------------------------------------------------
.thumb_func
.type	mpu_new_connection, %function
.global mpu_new_connection
mpu_new_connection:
    @ Preserva registradores e configura frame
    push    {r7, lr}
    sub     sp, sp, #24
    add     r7, sp, #8
    str     r0, [r7, #4]          @ Salva ponteiro de conexão
    
    @ Abre /dev/mem para acesso à memória
    ldr     r0, =str_dev_mem
    movw    r1, #4098             @ Flags: O_RDWR | O_SYNC
    movt    r1, #16
    bl      open
    mov     r3, r0
    str     r3, [r7, #8]          @ Salva file descriptor
    
    @ Verifica se open foi bem-sucedido
    ldr     r3, [r7, #8]
    cmp     r3, #0
    bge     open_success
    
    @ Erro ao abrir /dev/mem
    ldr     r0, =str_open_error
    bl      perror
    mov     r0, #1
    bl      exit
    
open_success:
    @ Mapeia memória física para o espaço do usuário
    ldr     r3, [r7, #8]
    str     r3, [sp, #0]          @ fd para mmap
    mov     r3, #0
    movt    r3, #65312            @ Endereço base: 0xFF000000
    str     r3, [sp, #4]          @ Offset para mmap
    mov     r0, #0                @ addr = NULL (kernel escolhe)
    mov     r1, #20480            @ len = 20K
    mov     r2, #3                @ prot = PROT_READ | PROT_WRITE
    mov     r3, #1                @ flags = MAP_SHARED
    bl      mmap
    mov     r3, r0
    str     r3, [r7, #12]         @ Salva ponteiro mapeado
    
    @ Verifica se mmap foi bem-sucedido
    ldr     r3, [r7, #12]
    cmp     r3, #-1
    bne     mmap_success
    
    @ Erro de mmap
    ldr     r0, =str_mmap_error
    bl      perror
    ldr     r3, [r7, #8]
    mov     r0, r3
    bl      close
    mov     r0, #1
    bl      exit
    
mmap_success:
    @ Configura a estrutura de conexão
    ldr     r3, [r7, #4]
    add     r2, r7, #8
    ldmia   r2, {r0, r1}          @ Carrega fd e ponteiro mapeado
    stmia   r3, {r0, r1}          @ Armazena na estrutura
    
    @ Retorna
    ldr     r0, [r7, #4]
    add     r7, r7, #16
    mov     sp, r7
    pop     {r7, pc}

@ --------------------------------------------------------------------
@ mpu_close_connection(Connection) -> void
@ --------------------------------------------------------------------
@ Fecha a conexão com a MPU e libera recursos
@
@ @param r0: ponteiro para Connection (struct)
@ --------------------------------------------------------------------
.thumb_func
.type	mpu_close_connection, %function
.global mpu_close_connection
mpu_close_connection:
    @ Preserva registradores e configura frame
    push    {r7, lr}
    sub     sp, sp, #8
    add     r7, sp, #0
    str     r0, [r7, #4]          @ Salva ponteiro de conexão
    
    @ Desmapeia a memória
    ldr     r3, [r7, #4]
    ldr     r3, [r3, #4]          @ Obtém o ponteiro mapeado
    mov     r0, r3
    mov     r1, #20480            @ len = 20K
    bl      munmap
    mov     r3, r0
    cmp     r3, #0
    beq     munmap_success
    
    @ Erro em munmap
    ldr     r0, =str_munmap_error
    bl      perror
    
munmap_success:
    @ Fecha o descritor de arquivo
    ldr     r3, [r7, #4]
    ldr     r3, [r3, #0]          @ Obtém o file descriptor
    mov     r0, r3
    bl      close
    
    @ Retorna
    add     r7, r7, #8
    mov     sp, r7
    pop     {r7, pc}

@ --------------------------------------------------------------------
@ mpu_build_base_cmd(unsigned, unsigned) -> void
@ --------------------------------------------------------------------
@ Constrói um comando base para a MPU
@
@ @param  r0: tipo
@ @param  r1: modo
@ @return r0: comando construído
@ --------------------------------------------------------------------
.thumb_func
.type	mpu_build_base_cmd, %function
.global mpu_build_base_cmd
mpu_build_base_cmd:
    @ Configura frame
    push    {r7}
    sub     sp, sp, #20
    add     r7, sp, #0
    str     r0, [r7, #4]          @ Salva tipo
    str     r1, [r7, #0]          @ Salva modo
    
    @ Máscara e shift do tipo (3 bits)
    ldr     r3, [r7, #4]
    and     r3, r3, #7            @ Máscara para 3 bits
    lsl     r3, r3, #27           @ Posiciona nos bits 27-29
    str     r3, [r7, #8]          @ Comando base
    
    @ Verifica se é tipo 5 (especial)
    ldr     r3, [r7, #4]
    cmp     r3, #5
    bne     build_done
    
    @ Se tipo=5, verifica se modo=2
    ldr     r3, [r7, #0]
    cmp     r3, #2
    bne     mode_not_2
    
    @ Tipo 5, modo 2 - define flag especial
    mov     r3, #1
    str     r3, [r7, #12]
    b       set_special_flag
    
mode_not_2:
    mov     r3, #0
    str     r3, [r7, #12]
    
set_special_flag:
    @ Adiciona flag especial ao comando (bit 30)
    ldr     r3, [r7, #12]
    lsl     r3, r3, #30           @ Posiciona no bit 30
    ldr     r2, [r7, #8]
    orrs    r3, r3, r2            @ Combina com comando base
    str     r3, [r7, #8]
    
build_done:
    @ Retorna o comando construído
    ldr     r3, [r7, #8]
    mov     r0, r3
    add     r7, r7, #20
    mov     sp, r7
    pop     {r7}
    bx      lr

@ --------------------------------------------------------------------
@ mpu_next_stage(PinIO, uint32_t) -> void
@ --------------------------------------------------------------------
@ Avança a MPU para o próximo estágio de operação
@
@ @param r0, r1: ponteiros para mapeamento PinIO (struct), 
@ @param r2: comando
@ --------------------------------------------------------------------
.thumb_func
.type	mpu_next_stage, %function
.global mpu_next_stage
mpu_next_stage:
    @ Preserva registradores e configura frame
    push    {r7, lr}
    sub     sp, sp, #16
    add     r7, sp, #0
    add     r3, r7, #8
    stmia   r3, {r0, r1}          @ Salva ponteiros
    str     r2, [r7, #4]          @ Salva comando
    
    @ Define bit 31 (início de comando)
    ldr     r3, [r7, #8]
    ldr     r2, [r7, #4]
    orr     r2, r2, #0x80000000   @ Define bit mais significativo
    str     r2, [r3, #0]          @ Escreve comando com bit de início
    
    @ Limpa bit 31 (executa comando)
    ldr     r3, [r7, #8]
    ldr     r2, [r7, #4]
    str     r2, [r3, #0]          @ Escreve comando sem bit de início
    
    @ Espera a execução
    bl      delay_microsecond
    
    @ Retorna
    add     r7, r7, #16
    mov     sp, r7
    pop     {r7, pc}

@ --------------------------------------------------------------------
@ mpu_store(Matrix, PinIO, uint32_t) -> void
@ --------------------------------------------------------------------
@ Armazena dados na MPU
@
@ @param r0:    ponteiro para matrix 5x5 (array 2d)
@ @param r1/r2: ponteiros para mapeamento PinIO (struct)
@ @param r3:    comando base
@ --------------------------------------------------------------------
.thumb_func
.type	mpu_store, %function
.global mpu_store
mpu_store:
    @ Preserva registradores e configura frame
    push    {r7, lr}
    sub     sp, sp, #40
    add     r7, sp, #0
    str     r0, [r7, #12]         @ Salva ponteiro de dados
    add     r0, r7, #4
    stmia   r0, {r1, r2}          @ Salva ponteiros de registradores
    str     r3, [r7, #0]          @ Salva comando base
    
    @ Loop para armazenar 200 bytes de dados (0-199)
    mov     r3, #0                @ Inicializa contador
    str     r3, [r7, #16]
    
store_loop:
    @ Calcula índices para matriz 5x5x8
    ldr     r3, [r7, #16]
    lsr     r3, r3, #3            @ Divide por 8 para obter posição da matriz
    str     r3, [r7, #20]
    ldr     r3, [r7, #16]
    and     r3, r3, #7            @ Resto da divisão por 8 para bit atual
    str     r3, [r7, #24]
    
    @ Calcula linha (divisão por 5)
    ldr     r2, [r7, #20]
    movw    r3, #52429
    movt    r3, 52428
    umull   r1, r3, r3, r2        @ Multiplicação por 0.2 aproximado (divisão por 5)
    lsr     r3, r3, #2            @ Finaliza divisão por 5
    str     r3, [r7, #28]
    
    @ Calcula coluna (módulo 5)
    ldr     r2, [r7, #20]
    movw    r3, #52429
    movt    r3, 52428
    umull   r1, r3, r3, r2
    lsr     r1, r3, #2
    mov     r3, r1
    lsl     r3, r3, #2
    adds    r3, r3, r1            @ Linha * 5
    subs    r3, r2, r3            @ Posição - (linha * 5) = coluna
    str     r3, [r7, #32]
    
    @ Obtém o byte na matriz
    ldr     r2, [r7, #28]
    mov     r3, r2
    lsl     r3, r3, #2
    adds    r3, r3, r2            @ linha * 5
    ldr     r2, [r7, #12]
    adds    r2, r2, r3            @ dados + (linha * 5)
    ldr     r3, [r7, #32]
    adds    r3, r2, r3            @ dados + (linha * 5) + coluna
    ldrb    r3, [r3, #0]          @ Carrega o byte
    mov     r2, r3
    
    @ Extrai o bit relevante
    ldr     r3, [r7, #24]
    asr     r3, r2, r3            @ Shift para obter o bit
    and     r3, r3, #1            @ Máscara para obter apenas o bit desejado
    str     r3, [r7, #36]
    
    @ Envia comando com o bit no barramento
    ldr     r3, [r7, #4]
    ldr     r2, [r7, #16]
    lsl     r1, r2, #1            @ Posição do bit * 2
    ldr     r2, [r7, #0]
    orrs    r1, r1, r2            @ Combina com comando base
    ldr     r2, [r7, #36]
    orrs    r2, r2, r1            @ Adiciona o valor do bit
    orr     r2, r2, #0x80000000   @ Define bit de início
    str     r2, [r3, #0]          @ Envia comando
    
    @ Espera execução
    bl      delay_microsecond
    
    @ Limpa bit de início
    ldr     r3, [r7, #4]
    ldr     r2, [r7, #0]
    str     r2, [r3, #0]
    
    @ Espera execução
    bl      delay_microsecond
    
    @ Incrementa contador e continua loop
    ldr     r3, [r7, #16]
    add     r3, r3, #1
    str     r3, [r7, #16]
    ldr     r3, [r7, #16]
    cmp     r3, #199              @ Loop até 200 bytes
    bls     store_loop
    
    @ Retorna
    add     r7, r7, #40
    mov     sp, r7
    pop     {r7, pc}

@ --------------------------------------------------------------------
@ mpu_load(Matrix, PinIO, uint32_t) -> void
@ --------------------------------------------------------------------
@ Carrega dados da MPU
@
@ @param r0:    ponteiro para matriz destino (array 2d)
@ @param r1/r2: ponteiros para mapeamento PinIO (struct)
@ @param r3:    comando base
@ --------------------------------------------------------------------
.thumb_func
.type	mpu_load, %function
.global mpu_load
mpu_load:
    @ Preserva registradores e configura frame
    push    {r7, lr}
    sub     sp, sp, #40
    add     r7, sp, #0
    str     r0, [r7, #12]         @ Salva ponteiro de buffer
    add     r0, r7, #4
    stmia   r0, {r1, r2}          @ Salva ponteiros de registradores
    str     r3, [r7, #0]          @ Salva comando base
    
    @ Loop para carregar 25 bytes de dados (0-24)
    mov     r3, #0                @ Inicializa contador
    str     r3, [r7, #20]
    
load_loop:
    @ Envia comando de leitura
    ldr     r3, [r7, #4]
    ldr     r2, [r7, #0]
    orr     r2, r2, #0x80000000   @ Define bit de início
    str     r2, [r3, #0]          @ Envia comando
    
    @ Limpa bit de início
    ldr     r3, [r7, #4]
    ldr     r2, [r7, #0]
    str     r2, [r3, #0]
    
    @ Espera execução
    bl      delay_microsecond
    
    @ Lê dados do registrador de status
    ldr     r3, [r7, #8]
    ldr     r3, [r3, #0]          @ Lê registrador de status
    str     r3, [r7, #24]
    
    @ Extrai posição da matriz (bits 26-30)
    ldr     r3, [r7, #24]
    lsr     r3, r3, #26
    and     r3, r3, #31           @ 5 bits para posição
    str     r3, [r7, #28]
    
    @ Calcula linha (divisão por 5)
    ldr     r2, [r7, #28]
    movw    r3, #52429
    movt    r3, 52428
    umull   r1, r3, r3, r2        @ Aproximação de divisão por 5
    lsr     r3, r3, #2
    str     r3, [r7, #32]
    
    @ Calcula coluna (módulo 5)
    ldr     r2, [r7, #28]
    movw    r3, #52429
    movt    r3, 52428
    umull   r1, r3, r3, r2
    lsr     r1, r3, #2
    mov     r3, r1
    lsl     r3, r3, #2
    adds    r3, r3, r1            @ linha * 5
    subs    r3, r2, r3            @ posição - (linha * 5) = coluna
    str     r3, [r7, #36]
    
    @ Armazena o byte no buffer
    ldr     r2, [r7, #32]
    mov     r3, r2
    lsl     r3, r3, #2
    adds    r3, r3, r2            @ linha * 5
    ldr     r2, [r7, #12]
    adds    r1, r2, r3            @ buffer + (linha * 5)
    ldr     r3, [r7, #24]
    uxtb    r2, r3                @ Extrai byte de dados
    ldr     r3, [r7, #36]
    adds    r3, r1, r3            @ buffer + (linha * 5) + coluna
    strb    r2, [r3, #0]          @ Armazena byte
    
    @ Avança para próximo byte
    ldr     r3, [r7, #4]
    ldr     r2, [r7, #0]
    orr     r2, r2, #0x04000000   @ Define bit para próximo byte (bit 26)
    str     r2, [r3, #0]
    
    @ Limpa bit de avanço
    ldr     r3, [r7, #4]
    ldr     r2, [r7, #0]
    str     r2, [r3, #0]
    
    @ Espera execução
    bl      delay_microsecond
    
    @ Incrementa contador e continua loop
    ldr     r3, [r7, #20]
    add     r3, r3, #1
    str     r3, [r7, #20]
    ldr     r3, [r7, #20]
    cmp     r3, #24               @ Loop até 25 bytes (matriz 5x5)
    bls     load_loop
    
    @ Retorna
    add     r7, r7, #40
    mov     sp, r7
    pop     {r7, pc}