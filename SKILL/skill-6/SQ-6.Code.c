#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 200

void processEscapes(char input[], char output[]) {
    int i = 0;
    int j = 0;
    int escaped = 0;

    while (input[i] != '\0') {
        if (escaped) {
            output[j] = input[i];
            j++;
            escaped = 0;
        }
        else if (input[i] == '\\') {
            escaped = 1;
        }
        else {
            output[j] = input[i];
            j++;
        }

        i++;
    }

    output[j] = '\0';
}

int main() {
    char input[MAX];
    char output[MAX];

    printf("====================================\n");
    printf("     PRACTICAL 6 - PROCESS DEMO\n");
    printf("====================================\n");

    /* Escape sequence processing */
    printf("\nEnter a string with escape characters:\n");
    printf("Example: hello\\ world\\!\\ test\n");
    printf("Input: ");

    fgets(input, MAX, stdin);

    input[strcspn(input, "\n")] = '\0';

    processEscapes(input, output);

    printf("\nOriginal Input : %s\n", input);
    printf("Parsed Output  : %s\n", output);

    /* Create child process */
    printf("\n------------------------------------\n");
    printf("Creating Child Process...\n");
    printf("------------------------------------\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        /* Child process */

        printf("\nChild Process\n");
        printf("Child PID  : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());

        printf("\nExecuting ls -l command...\n\n");

        execlp("ls", "ls", "-l", NULL);

        /* Runs only if execlp fails */
        perror("exec failed");
        exit(1);
    }
    else {
        /* Parent process */

        int status;

        printf("\nParent Process\n");
        printf("Parent PID : %d\n", getpid());
        printf("Child PID  : %d\n", pid);

        printf("\nParent waiting for child...\n");

        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return 1;
        }

        if (WIFEXITED(status)) {
            printf("\nChild process finished.\n");
            printf("Exit status: %d\n", WEXITSTATUS(status));
        }
        else {
            printf("\nChild process terminated abnormally.\n");
        }
    }

    printf("\n====================================\n");
    printf("Program completed successfully.\n");
    printf("====================================\n");

    return 0;
}
