#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    // Read input from stdin
    printf("Enter command: ");
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        perror("fgets failed");
        return EXIT_FAILURE;
    }

    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;

    // Find the pipe symbol '|'
    char *command1 = strtok(input, "|");
    char *command2 = strtok(NULL, "|");

    if (command1 == NULL || command2 == NULL) {
        fprintf(stderr, "Error: Command must contain a pipe '|'\n");
        return EXIT_FAILURE;
    }

    // Trim leading and trailing spaces
    while (*command1 == ' ') command1++;
    while (*command2 == ' ') command2++;

    // Create pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return EXIT_FAILURE;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid1 == 0) {
        // Child process 1 - Executes first command
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]); // Close pipe write end

        // Execute command
        execlp("/bin/sh", "sh", "-c", command1, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid2 == 0) {
        // Child process 2 - Executes second command
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
        close(pipefd[0]); // Close pipe read end

        // Execute command
        execlp("/bin/sh", "sh", "-c", command2, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return EXIT_SUCCESS;
}

