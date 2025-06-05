#include <app/matrix.h>


Bridge connect() {
    return (Bridge) {
        #ifdef IN_PROD
            .connection = mpu_new_connection(),
        #else
            .connection = {0},
        #endif
        .connected = true
    };
}

void disconnect(Bridge *bridge) {
    #ifdef IN_PROD
        mpu_close_connection(&bridge->connection);
    #endif
    bridge->connected = false; 
}