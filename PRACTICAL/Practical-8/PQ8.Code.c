#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE (10 * 1024 * 1024)

char data[SIZE];

void print_memory(const char *process)
{
    char command[100];

    printf("\n%s Process - PID: %d\n", process, getpid());

    snprintf(command, sizeof(command),
             "grep VmRSS /proc/%d/status", getpid());

    system(command);
}

int main()
{
    // Initialize data
    for (long i = 0; i < SIZE; i++)
        data[i] = 'A';

    printf("Parent process before fork:\n");
    print_memory("Parent");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        // Child process
        printf("\nChild process created.\n");

        print_memory("Child before modification");

        printf("\nChild modifying memory...\n");

        // Modify a large portion of the memory
        for (long i = 0; i < SIZE; i++)
            data[i] = 'B';

        print_memory("Child after modification");

        printf("Child data[0] = %c\n", data[0]);

        exit(0);
    }
    else
    {
        // Parent process
        sleep(2);

        print_memory("Parent while child is running");

        wait(NULL);

        printf("\nChild process completed.\n");

        printf("Parent data[0] = %c\n", data[0]);

        print_memory("Parent after child exits");
    }

    return 0;
}
