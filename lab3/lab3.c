#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Parse input into commands and arguments
void parse_input (char *input, char *args[], int *background)
{
    int i = 0;
    *background = 0;

    //Tokenize the input
    char *token = strtok(input, " \n");
    while (token != NULL) 
    {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }

    //If the last argument is &, it should be running in the background
    if (i > 0 && strcmp(args[i - 1], "&") == 0)
    {
        *background = 1;
        args[i - 1] = NULL;
    }
    else 
    {
        args[i] = NULL;
    }
}

//Executes the command with fork and execvp
void execute_command (char *args[], int background)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed!");
        exit(1);
    }
    //Child process
    else if (pid == 0 && execvp(args[0], args) < 0)
    {
        perror("Command excecution unsucessful");
        exit(1);
    }

    //Parent process
    else if (!background)
    {
        waitpid(pid, NULL, 0);
    }
}

//Main method
int main ()
{
    char input[100];
    char *args[10];
    int background;

    //Infinite loop for the shell
    while (1)
    {
        printf("Your command> ");
        fflush(stdout);

        //Break if the end of the file is reached
        if (fgets(input, 10, stdin) == NULL)
        {
            break;
        }

        parse_input(input, args, &background);

        //Continue if the input is empty
        if (args[0 == NULL])
        {
            continue;
        }

        //Break if the user types exit
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        execute_command(args, background);

    }

    printf("Exiting shell...\n");
    return 0;
}