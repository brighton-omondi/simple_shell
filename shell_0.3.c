#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define DELIMITER " \t\n"

/**
 * main - Entry point for the simple shell program
 *
 * Return: Always 0 on success
 */
int main(void)
{
    char buffer[BUFFER_SIZE];
    pid_t pid;
    int status;

    while (1)
    {
        printf(":) ");
        if (!fgets(buffer, BUFFER_SIZE, stdin))
        {
            printf("\n");
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline

        char *token;
        char *args[BUFFER_SIZE];
        int arg_count = 0;

        // Tokenize the command line input
        token = strtok(buffer, DELIMITER);
        while (token != NULL)
        {
            args[arg_count++] = token;
            token = strtok(NULL, DELIMITER);
        }
        args[arg_count] = NULL; // Set the last argument to NULL

        // Get the PATH environment variable
        char *path = getenv("PATH");
        if (path == NULL)
        {
            perror("getenv");
            exit(EXIT_FAILURE);
        }

        // Tokenize the PATH variable
        char *path_token;
        path_token = strtok(path, ":");
        int found = 0;
        while (path_token != NULL)
        {
            printf("Searching in directory: %s\n", path_token); // Debug print

            // Construct the full path to the executable
            char command[BUFFER_SIZE];
            snprintf(command, BUFFER_SIZE, "%s/%s", path_token, args[0]);

            // Check if the executable exists
            if (access(command, X_OK) == 0)
            {
                found = 1;
                pid = fork();
                if (pid < 0)
                {
                    perror("Fork error");
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    // Child process
                    if (execv(command, args) == -1)
                    {
                        perror(args[0]);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // Parent process
                    waitpid(pid, &status, 0);
                    break;
                }
            }

            path_token = strtok(NULL, ":");
        }

        // If the command doesn't exist in any directory in the PATH
        if (!found)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
        }
    }

    return EXIT_SUCCESS;
}
