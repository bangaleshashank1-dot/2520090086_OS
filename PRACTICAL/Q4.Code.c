#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1, pid2;
    int status;

    printf("Parent Process ID: %d\n", getpid());

    pid1 = fork();

    if (pid1 == 0)
    {
        printf("Child 1: PID = %d\n", getpid());
        sleep(2);
        printf("Child 1 Completed\n");
        exit(10);
    }

    pid2 = fork();

    if (pid2 == 0)
    {
        printf("Child 2: PID = %d\n", getpid());
        sleep(4);
        printf("Child 2 Completed\n");
        exit(20);
    }

    printf("\nParent waiting using wait()...\n");

    pid_t child = wait(&status);

    printf("wait() collected Child PID = %d, Exit Status = %d\n",
           child, WEXITSTATUS(status));

    printf("\nParent waiting using waitpid()...\n");

    waitpid(pid2, &status, 0);

    printf("waitpid() collected Child PID = %d, Exit Status = %d\n",
           pid2, WEXITSTATUS(status));

    printf("\nAll child processes completed.\n");

    return 0;
}
