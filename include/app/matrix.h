#ifndef APP_MATRIX_H
#define APP_MATRIX_H

#include <stdbool.h>
#include <stdint.h>

#define DIM    5
#define PIO_CMD_OFFSET 0
#define PIO_STAT_OFFSET 0

typedef uint8_t Matrix[DIM][DIM];

typedef struct {
    volatile uint32_t *cmd;
    volatile uint32_t *stat;
} PinIO;

typedef struct {
    int   fd;
    void *base;
} Connection;

typedef struct  {
    Connection connection;
    bool connected;
} Bridge;

Bridge connect();

void disconnect(Bridge *bridge);

#define with_connection(bridge)   \
    for (Bridge bridge = connect(); bridge.connected; disconnect(&bridge))

#endif  // APP_MATRIX_H
