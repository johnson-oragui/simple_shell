/* execution.c */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void execute_command(char **args)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

int tokenize_input(char *input, char **args)
{
    // Implementation of input tokenization
     int i = 0;
    char *token = strtok(input, " \t\n\r");

    while (token != NULL)
    {
        args[i] = token;
        i++;

        token = strtok(NULL, " \t\n\r");
    }

    args[i] = NULL;

    return i;
}

void run_commands_from_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }

    char line[MAX_INPUT_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove trailing newline character from line
        line[strcspn(line, "\n")] = '\0';

        // Tokenize line
        char *args[MAX_ARGS];
        int num_args = tokenize_input(line, args);

        if (num_args > 0) {
            replace_variables(args);
            execute_command(args);
        }
    }

    fclose(file);
}
