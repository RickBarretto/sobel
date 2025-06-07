#ifndef APP_MATRIX_H
#define APP_MATRIX_H

#include <stdbool.h>
#include <stdint.h>

#include <mpu/mpu.h>

typedef struct  {
    Connection connection;
    bool connected;
} Bridge;

Bridge connect();

void disconnect(Bridge *bridge);

#define with_connection(bridge)   \
    for (Bridge bridge = connect(); bridge.connected; disconnect(&bridge))

#endif  // APP_MATRIX_H
