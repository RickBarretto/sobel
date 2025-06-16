module MpuMain (
  input  wire        clock,
  input  wire        reset_n,
  input  wire [31:0] pio_out,
  output wire [31:0] pio_in,    // { flag, byte_idx, zeros, data_byte }
  output wire        LED_0,
  output wire        LED_1,
  output wire        LED_2
);

  localparam N_BITS   = 200;
  localparam N_BYTES  = 25;

  localparam IDLE     = 3'd0,
             LOAD_A   = 3'd1,
             LOAD_B   = 3'd2,
             EXEC_OP  = 3'd3,
             READ_RES = 3'd4;

  reg  [2:0] state;
  reg  [7:0] load_cnt;
  reg  [4:0] byte_idx;
  reg  [2:0] ops_reg;
  reg  [7:0] data_out_reg;
  reg        flag_reg;
  reg        start_prev, ack_prev;
  integer    cycle_counter;

  reg signed [N_BITS-1:0] Matrix_A, Matrix_B;
  wire signed [N_BITS-1:0] Result;

  reg [7:0] matrix_size_reg;

  wire start_pulse   = pio_out[31] & ~start_prev;
  wire ack_pulse     = pio_out[26] & ~ack_prev;
  wire [2:0] op_code  = pio_out[29:27];
  wire [7:0] bit_pos  = pio_out[8:1];
  wire       bit_val  = pio_out[0];

  assign LED_0 = (state == LOAD_A);
  assign LED_1 = (state == LOAD_B);
  assign LED_2 = (state == READ_RES);

  assign pio_in = { flag_reg, byte_idx, 18'd0, data_out_reg };

  MpuOperations op_inst (
    .operation (ops_reg),
    .matrix_a  (Matrix_A),
    .matrix_b  (Matrix_B),
    .size      (matrix_size_reg? 8'd2 : 8'd3),
    .factor    (Matrix_B[7:0]),
    .clock     (clock),
    .result    (Result),
	 .start     (state == EXEC_OP)
  );

  always @(posedge clock or negedge reset_n) begin
    if (!reset_n) begin
      state            <= IDLE;
      load_cnt         <= 0;
      byte_idx         <= 0;
      ops_reg          <= 0;
      data_out_reg     <= 0;
      flag_reg         <= 0;
      start_prev       <= 0;
      ack_prev         <= 0;
      cycle_counter    <= 0;
      Matrix_A         <= 0;
      Matrix_B         <= 0;
      matrix_size_reg  <= 0;
    end else begin
      start_prev <= pio_out[31];
      ack_prev   <= pio_out[26];

      case (state)
        IDLE: begin
          if (start_pulse) begin
            ops_reg         <= op_code;
            matrix_size_reg <= {7'b0, pio_out[30]};
            load_cnt        <= 0;
            state           <= LOAD_A;
          end
        end

        LOAD_A: begin
          if (start_pulse) begin
            Matrix_A[bit_pos] <= bit_val;
            load_cnt <= load_cnt + 1;
            if (load_cnt == N_BITS-1) begin
              load_cnt <= 0;
              state    <= LOAD_B;
            end
          end
        end

        LOAD_B: begin
          if (start_pulse) begin
            Matrix_B[bit_pos] <= bit_val;
            load_cnt <= load_cnt + 1;
            if (load_cnt == N_BITS-1) begin
              state <= EXEC_OP;
            end
          end
        end

        EXEC_OP: begin
          cycle_counter <= cycle_counter + 1;
          if (cycle_counter > 9) begin
            cycle_counter <= 0;
            byte_idx      <= 0;
            state         <= READ_RES;
          end
        end

        READ_RES: begin
          if (!flag_reg) begin
            data_out_reg <= Result[ byte_idx*8 +: 8 ];
            flag_reg     <= 1;
          end else if (ack_pulse) begin
            flag_reg <= 0;
            if (byte_idx == N_BYTES-1)
              state <= IDLE;
            else
              byte_idx <= byte_idx + 1;
          end
        end

        default: state <= IDLE;
      endcase
    end
  end

endmodule