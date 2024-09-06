#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <serial.h>

// Function to open the serial port
FILE* open_file(const char* portname, char* mode) {
    FILE* file_ptr = fopen(portname, mode);
    if (file_ptr == NULL) {
        printf("Error opening %s : %d\n", portname, errno);
        return NULL;
    }
    return file_ptr;
}

// Function to read data from SBUS
void read_SBUS(uint8_t* buffer, size_t size, size_t n, FILE* fd) {
    int rc = fread(buffer, size, n, fd);
    if (rc != n) {
        printf("Error reading SBUS data: %d\n", errno);
    }
}

// Function to write data to Sabertooth
void write_to_SB(FILE* fd, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int rc = vfprintf(fd, fmt, args);
    va_end(args);
    if (rc < 0) {
        printf("Error writing to Sabertooth: %d\n", errno);
    }
}

// Function to close the serial port
void close_file(FILE* fd) {
    fclose(fd);
}
