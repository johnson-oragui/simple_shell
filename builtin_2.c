/* builtin_2.c */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ALIAS_COUNT 100
#define MAX_VARIABLE_COUNT 100

struct Alias
{
    char name[100];
    char value[100];
};

int alias_count = 0;
struct Alias aliases[MAX_ALIAS_COUNT];

int variable_count = 0;
char *variables[MAX_VARIABLE_COUNT];
int exit_status = 0;


int get_exit_status()
{
    extern int exit_status;
    return exit_status;
}

void print_aliases()
{
    int i;

    for (i = 0; i < alias_count; i++) {
        printf("%s='%s'\n", aliases[i].name, aliases[i].value);
    }
}

void print_alias(char *name)
{
    int i;

    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            printf("%s='%s'\n", aliases[i].name, aliases[i].value);
            return;
        }
    }

    fprintf(stderr, "Alias '%s' not found\n", name);
}

void define_alias(char *name, char *value)
{
    int i;

    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            strcpy(aliases[i].value, value);
            return;
        }
    }

    if (alias_count < MAX_ALIAS_COUNT) {
        strcpy(aliases[alias_count].name, name);
        strcpy(aliases[alias_count].value, value);
        alias_count++;
    } else {
        fprintf(stderr, "Maximum number of aliases reached\n");
    }
}

void print_variables()
{
    int i;

    for (i = 0; i < variable_count; i++) {
        printf("%s=%s\n", variables[i], getenv(variables[i]));
    }
}

char *get_variable_value(char *name)
{
    int i;

    for (i = 0; i < variable_count; i++) {
        if (strcmp(variables[i], name) == 0) {
            return getenv(variables[i]);
        }
    }

    return NULL;
}

void define_variable(char *name)
{
    if (variable_count < MAX_VARIABLE_COUNT) {
        strcpy(variables[variable_count], name);
        variable_count++;
    } else {
        fprintf(stderr, "Maximum number of variables reached\n");
    }
}

void replace_variables(char **args)
{
    int  i;

    for (i = 0; args[i] != NULL; i++) {
        char *arg = args[i];
        int length = strlen(arg);
        char *variable = NULL;
        char *value = NULL;
        int replace = 0;

        /* Check if arg starts with '$' */
        if (arg[0] == '$') {
            /* Check if arg is '$?' */
            if (strcmp(arg, "$?") == 0) {
                /* Replace with exit status value */
                char exit_status[4];
                sprintf(exit_status, "%d", get_exit_status());
                value = strdup(exit_status);
                replace = 1;
            }
            /* Check if arg is '$$' */
            else if (strcmp(arg, "$$") == 0) {
                /* Replace with process ID value */
                char process_id[16];
                sprintf(process_id, "%d", getpid());
                value = strdup(process_id);
                replace = 1;
            }
            /* Check if arg is a variable */
            else {
                variable = &arg[1];
                value = get_variable_value(variable);
                replace = 1;
            }
        }

        if (replace) {
            /* Replace arg with value if it exists */
            if (value != NULL) {
                free(args[i]);
                args[i] = strdup(value);
                free(value);
            }
            /* Print error message if variable doesn't exist */
            else if (variable != NULL) {
                fprintf(stderr, "Variable '%s' not found\n", variable);
            }
        }
    }
}
