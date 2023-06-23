// builtin.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

void shell_exit(char **args)
{
    // Check if an argument is provided
    if (args[1] != NULL)
    {
        // Convert the argument to an integer
        int status = atoi(args[1]);

        // Exit the shell with the specified status
        exit(status);
    }
    else
    {
        // Exit the shell with the default status
        exit(EXIT_SUCCESS);
    }
}

void shell_setenv(char **args)
{
    // Check if both variable and value arguments are provided
    if (args[1] != NULL && args[2] != NULL)
    {
        // Set the environment variable
        if (setenv(args[1], args[2], 1) != 0)
        {
            fprintf(stderr, "Failed to set environment variable\n");
        }
    }
    else
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
    }
}

void shell_unsetenv(char **args)
{
    // Check if a variable argument is provided
    if (args[1] != NULL)
    {
        // Unset the environment variable
        if (unsetenv(args[1]) != 0)
        {
            fprintf(stderr, "Failed to unset environment variable\n");
        }
    }
    else
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
    }
}

void shell_cd(char **args)
{
    // Check if a directory argument is provided
    if (args[1] != NULL)
    {
        // Change directory to the specified directory
        if (chdir(args[1]) != 0)
        {
            perror("cd");
        }
        else
        {
            // Update the PWD environment variable
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                setenv("PWD", cwd, 1);
            }
            else
            {
                perror("getcwd");
            }
        }
    }
    else
    {
        // If no argument is provided, change directory to the home directory
        char *home = getenv("HOME");
        if (home != NULL)
        {
            if (chdir(home) != 0)
            {
                perror("cd");
            }
            else
            {
                // Update the PWD environment variable
                setenv("PWD", home, 1);
            }
        }
        else
        {
            fprintf(stderr, "cd: No home directory found\n");
        }
    }
}

void shell_alias(char **args)
{
    // Check if any arguments are provided
    if (args[1] != NULL)
    {
        // If no argument is provided after "alias", print all aliases
        if (args[1][0] == '\0')
        {
            // Print all aliases
            // ...
        }
        else
        {
            // Parse and handle individual aliases
            int i = 1;
            while (args[i] != NULL)
            {
                // Handle individual alias
                // ...
                i++;
            }
        }
    }
    else
    {
        fprintf(stderr, "Usage: alias [name[='value'] ...]\n");
    }
}

