#include "shell.h"
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

char *custom_getline()
{
    static char buffer[BUFFER_SIZE];  // Static buffer to hold input
    static size_t buffer_pos = 0;     // Current position in the buffer
    static ssize_t bytes_read = 0;    // Number of bytes read from input
    static ssize_t buffer_size = 0;   // Size of the buffer

    char *line = NULL;
    size_t line_size = 0;
    ssize_t bytes_to_read = 0;

    while (1) {
        // Check if buffer is empty
        if (buffer_pos >= bytes_read) {
            // Read more input into the buffer
            bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            buffer_pos = 0;

            // Check for end of file or read error
            if (bytes_read <= 0)
                return NULL;
        }

        // Find the number of bytes to read from the buffer
        bytes_to_read = bytes_read - buffer_pos;

        // Iterate through the buffer to find newline or end of buffer
        for (ssize_t i = 0; i < bytes_to_read; i++) {
            if (buffer[buffer_pos + i] == '\n') {
                // Allocate memory for the line
                line = realloc(line, line_size + i + 1);
                if (line == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                // Copy the line from the buffer
                memcpy(line + line_size, buffer + buffer_pos, i);
                line[line_size + i] = '\0';

                // Update buffer position
                buffer_pos += i + 1;

                // Return the line
                return line;
            }
        }

        // Allocate memory for the line and copy the buffer contents
        line = realloc(line, line_size + bytes_to_read);
        if (line == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        // Copy the buffer contents to the line
        memcpy(line + line_size, buffer + buffer_pos, bytes_to_read);

        // Update line size and buffer position
        line_size += bytes_to_read;
        buffer_pos += bytes_to_read;
    }
}

