/* shell.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int main(int argc, char **argv)
{
    if (argc > 1) {
        run_commands_from_file(argv[1]);
        return 0;
    }

    char input[MAX_INPUT_LENGTH];

    while (1) {
        printf("$ ");
        fgets(input, MAX_INPUT_LENGTH, stdin);

        // Remove trailing newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Check if input is a comment
        if (input[0] == '#') {
            continue;
        }

        // Tokenize input
        char *args[MAX_ARGS];
        int num_args = tokenize_input(input, args);

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

    return 0;
}
