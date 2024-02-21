#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> // Include string.h for strcspn function

#define BUFFER_SIZE 1024

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
        printf("#cisfun$ ");
        if (!fgets(buffer, BUFFER_SIZE, stdin))
        {
            printf("\n");
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline

        pid = fork();
        if (pid < 0)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process
            if (execlp(buffer, buffer, (char *)NULL) == -1)
            {
                perror(buffer);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // Parent process
            waitpid(pid, &status, 0);
        }
    }

    return EXIT_SUCCESS;
}
