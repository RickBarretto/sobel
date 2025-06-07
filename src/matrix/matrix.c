#include <app/matrix.h>


Bridge connect() {
    return (Bridge) {
        .connection = {0},
        .connected = true
    };
}

void disconnect(Bridge *bridge) {
    bridge->connected = false; 
}