#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char command[100];

    printf("Enter Linux command: ");
    scanf("%s", command);

    pid_t pid = fork();

    if (pid == 0) {
        printf("Child PID: %d\n", getpid());
        execlp(command, command, NULL);
        perror("Execution Failed");
    }
    else if (pid > 0) {
        printf("Parent PID: %d\n", getpid());
        wait(NULL);
        printf("Child Completed\n");
    }
    else {
        printf("Fork Failed\n");
    }

    return 0;
}
