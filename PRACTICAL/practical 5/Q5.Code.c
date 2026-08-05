#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    char message[] = "Hello from Parent Process!";
    char buffer[100];

    struct timeval start, end;

    pipe(fd);

    gettimeofday(&start, NULL);

    if(fork() == 0)
    {
        close(fd[1]);

        read(fd[0], buffer, sizeof(buffer));

        printf("Child received: %s\n", buffer);

        close(fd[0]);

        exit(0);
    }
    else
    {
        close(fd[0]);

        write(fd[1], message, strlen(message)+1);

        close(fd[1]);

        wait(NULL);

        gettimeofday(&end, NULL);

        double time =
        (end.tv_sec-start.tv_sec)*1000.0 +
        (end.tv_usec-start.tv_usec)/1000.0;

        printf("Communication Time = %.3f ms\n", time);
    }

    return 0;
}
