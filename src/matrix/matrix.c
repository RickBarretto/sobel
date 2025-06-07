#include <app/matrix.h>


Bridge connect() {
    return (Bridge) {
        .connection = mpu_new_connection(),
        .connected = true
    };
}

void disconnect(Bridge *bridge) {
    mpu_close_connection(&bridge->connection);
    bridge->connected = false; 
}