#include "buffer_data.h"
#include <stdio.h>

//returns bytes written to buffer
uint32_t buffer_data(char *buffer, packet_t *current_packet) { 
    snprintf(buffer, sizeof(current_packet), "");
}