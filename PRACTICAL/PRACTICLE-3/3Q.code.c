#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t pid = fork();

    if(pid == 0){

        printf("\nChild Process\n");
        printf("PID : %d\n", getpid());
        printf("PPID: %d\n", getppid());

        sleep(5);

        printf("Child Terminated\n");

    }

    else{

        printf("\nParent Process\n");
        printf("PID : %d\n", getpid());
        printf("Child PID : %d\n", pid);

        wait(NULL);

        printf("Parent Finished\n");

    }

    return 0;
}
