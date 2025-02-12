#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_INPUT 1024

int main()
{
    char input[MAX_INPUT];
    printf("Enter your command> ");

    // Reading input with fgets and returning an error if fgets fails
    if (fgets(input, MAX_INPUT, stdin) == NULL)
    {
        perror("fgets failed!");
        return EXIT_FAILURE;
    }

    // Finds index of "\n" in input and replaces it with a 0 to terminate the string.
    input[strcspn(input, "\n")] = 0;

    char *command1 = strtok(input, "|");
    char *command2 = strtok(NULL, "|");

    // Error if no pipe is found and the input isnt tokenized.
    if (command1 == NULL || command2 == NULL)
    {
        fprintf(stderr, "Error: Commnd must contain a pipe ( | )\n");
        return EXIT_FAILURE;
    }

    // Remove whitespace
    while (*command1 == ' ')
    {
        command1++;
    }
    while (*command2 == ' ')
    {
        command2++;
    }

    // Array for entry and exit of pipe
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe failed!");
        return EXIT_FAILURE;
    }

    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork failed!");
        return EXIT_FAILURE;
    }

    if (pid1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execlp("/bin/sh", "sh", "-c", command1, NULL);
        perror("execlp failed!");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork failed!");
        return EXIT_FAILURE;
    }

    if (pid2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execlp("/bin/sh", "sh", "-c", command2, NULL);
        perror("execlp failed!");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return EXIT_SUCCESS;
}
