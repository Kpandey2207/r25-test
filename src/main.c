/*
Errors/Changes Made:

1. Buffer Size for SBUS Data: 
SBUS packets are 25 bytes long, but you're only allocating 15 bytes (uint8_t sbus_packet[15]). 
You should allocate a 25-byte buffer.

2. Incorrect Use of read_SBUS: In the call to read_SBUS, you’ve used uint8_t as the size argument, 
which is incorrect. The size argument in fread expects the size of each element being read. 
You should pass sizeof(uint8_t) (or just 1) instead of uint8_t.

3. Port Name Check: Ensure that argc >= 3 before accessing argv[1] and argv[2]. Otherwise,
there might be an out-of-bounds access if not enough arguments are provided.

4. Error Handling: If the serial port fails to open, your program should handle the error 
gracefully (exit or display an error message).

*/

#include <stdint.h>
#include <stdio.h>
#include <serial.h>
#include <parsing.h>

int main(int argc, char** argv) {
    // Check if the correct number of arguments is provided
    if (argc < 3) {
        printf("Usage: %s <SBUS port> <Sabertooth port>\n", argv[0]);
        return -1;
    }

    // Port names from command-line arguments
    char *port_name_1 = argv[1]; // SBUS 
    char *port_name_2 = argv[2]; // Sabertooth1

    FILE *sbus; 
    FILE *sabertooth;

    // Buffer to store SBUS packets
    uint8_t sbus_packet[25]; // SBUS packet size is 25 bytes

    // Pointer to store parsed SBUS channel values
    uint16_t *channel;

    // Variable to store the interpolated PWM value
    int pwm;

    // Open serial port for SBUS and Sabertooth communication
    sbus = open_file(port_name_1, "rb");
    sabertooth = open_file(port_name_2, "w+");

    if (sbus == NULL || sabertooth == NULL) {
        printf("Error opening serial ports.\n");
        return -1;
    }

    // Read data from RC transmitter using SBUS
    read_SBUS(sbus_packet, sizeof(uint8_t), 25, sbus);

    // Parse the SBUS packet to extract channel data
    channel = parse_buffer(sbus_packet);

    // Get the interpolated PWM value for Sabertooth Motor 1 (Channel 0)
    pwm = interpolation(channel[0]);

    // Write the PWM value to Sabertooth Motor 1
    write_to_SB(sabertooth, "%d\n", pwm);

    // Close both serial ports
    close_file(sbus);
    close_file(sabertooth);

    return 0;
}