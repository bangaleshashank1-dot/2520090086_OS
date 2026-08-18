#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    printf("===== PROCESS DEMONSTRATION =====\n");

    printf("Main Process PID  : %d\n", getpid());
    printf("Main Parent PID   : %d\n", getppid());

    printf("\nCalling fork()...\n");

    pid = fork();

    if (pid < 0)
    {
        printf("Fork failed!\n");
        return 1;
    }

    else if (pid == 0)
    {
        // Child process
        printf("\n--- CHILD PROCESS ---\n");
        printf("Child PID         : %d\n", getpid());
        printf("Parent PID        : %d\n", getppid());

        printf("\nChild is executing 'ls -l' using exec()...\n");

        execl("/bin/ls", "ls", "-l", NULL);

        // This executes only if exec() fails
        perror("exec failed");
        return 1;
    }

    else
    {
        // Parent process
        printf("\n--- PARENT PROCESS ---\n");
        printf("Parent PID        : %d\n", getpid());
        printf("Child PID         : %d\n", pid);

        printf("\nParent is waiting for child...\n");

        wait(NULL);

        printf("Child process completed.\n");
        printf("Parent process completed.\n");
    }

    return 0;
}
