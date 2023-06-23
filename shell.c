#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 128

int main(int argc, char **argv)
{
    FILE *input_stream;
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARGS];
    int num_args;

    if (argc > 1) {
        run_commands_from_file(argv[1], argv[0]);
        return 0;
    }


    if (isatty(fileno(stdin))) {
        input_stream = stdin;
    } else {
        input_stream = fopen("/dev/tty", "r");
        if (input_stream == NULL) {
            perror("fopen");
            return 1;
        }
    }

    while (1)
    {
        printf("$ ");

        /* Read input */
        if (fgets(input, MAX_INPUT_LENGTH, input_stream) == NULL) {
	   /* Break loop on input EOF */
            break;
        }
        /* Remove trailing newline character from input */
        input[strcspn(input, "\n")] = '\0';

        /* Tokenize input */
	num_args = tokenize_input(input, args);

        /* Check if input is a comment */
        if (input[0] == '#') {
            continue;
        }

        if (num_args > 0) {
            if (strcmp(args[0], "exit") == 0) {
                shell_exit(args);
                break;
            } else if (strcmp(args[0], "setenv") == 0) {
                shell_setenv(args);
            } else if (strcmp(args[0], "unsetenv") == 0) {
                shell_unsetenv(args);
            } else if (strcmp(args[0], "cd") == 0) {
                shell_cd(args);
            } else if (strcmp(args[0], "alias") == 0) {
                shell_alias(args);
            } else {
                replace_variables(args);
                execute_command(args);
            }
        }
    }

    if (input_stream != stdin) {
        fclose(input_stream);
    }

    return 0;
}

