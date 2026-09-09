#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig)
{
    printf("\nSIGINT signal received!\n");
}

int main()
{
    signal(SIGINT, signal_handler);

    printf("Process ID: %d\n", getpid());
    printf("Waiting for SIGINT...\n");

    while (1)
    {
        printf("Program is running...\n");
        sleep(3);
    }

    return 0;
}
