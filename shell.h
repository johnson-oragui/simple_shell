/* shell.h */

#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 128

extern int exit_status;

/* Function prototypes */
void shell_exit(char **args);
void shell_setenv(char **args);
void shell_unsetenv(char **args);
void shell_cd(char **args);
void shell_alias(char **args);
void execute_command(char **args);
void run_commands_from_file(char *filename, char *program_name);
int tokenize_input(char *input, char **args);
void replace_variables(char **args);
int get_exit_status();
char* get_variable_value(char *name);


#endif

