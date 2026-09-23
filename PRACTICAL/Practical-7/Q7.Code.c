#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global variable
int global_var = 100;

// Static global variable
static int static_global_var = 200;

// Function (code segment)
void code_function()
{
    printf("Address of code function : %p\n", (void *)code_function);
}

int main()
{
    // Local variable (stack)
    int stack_var = 300;

    // Static local variable (data segment)
    static int static_var = 400;

    // Heap variable
    int *heap_var = (int *)malloc(sizeof(int));
    *heap_var = 500;

    printf("========================================\n");
    printf("   Linux Process Memory Address Layout\n");
    printf("========================================\n");

    // Code segment
    printf("Address of code function : %p\n",
           (void *)code_function);

    // Global segment
    printf("Address of global variable : %p\n",
           (void *)&global_var);

    // Static segment
    printf("Address of static global : %p\n",
           (void *)&static_global_var);

    printf("Address of static local  : %p\n",
           (void *)&static_var);

    // Heap segment
    printf("Address of heap variable : %p\n",
           (void *)heap_var);

    // Stack segment
    printf("Address of stack variable: %p\n",
           (void *)&stack_var);

    printf("\nProcess ID (PID): %d\n", getpid());

    printf("\n========================================\n");
    printf("   /proc/<PID>/maps Memory Mappings\n");
    printf("========================================\n\n");

    // Open /proc/<PID>/maps
    char maps_file[50];
    snprintf(maps_file, sizeof(maps_file),
             "/proc/%d/maps", getpid());

    FILE *fp = fopen(maps_file, "r");

    if (fp == NULL)
    {
        perror("Unable to open /proc/<PID>/maps");
        free(heap_var);
        return 1;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }

    fclose(fp);

    free(heap_var);

    return 0;
}
