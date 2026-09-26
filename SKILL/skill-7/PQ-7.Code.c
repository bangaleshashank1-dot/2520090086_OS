#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_PATH 4096
#define MAX_CMD 100

/* Find executable in PATH */
void findExecutable(char *command) {
    char *path;
    char *pathCopy;
    char *directory;
    char fullPath[MAX_PATH];
    struct stat fileInfo;

    path = getenv("PATH");

    if (path == NULL) {
        printf("PATH variable not found.\n");
        return;
    }

    pathCopy = malloc(strlen(path) + 1);

    if (pathCopy == NULL) {
        perror("Memory allocation failed");
        return;
    }

    strcpy(pathCopy, path);

    directory = strtok(pathCopy, ":");

    while (directory != NULL) {

        snprintf(fullPath, sizeof(fullPath),
                 "%s/%s", directory, command);

        if (stat(fullPath, &fileInfo) == 0) {

            if (access(fullPath, X_OK) == 0) {
                printf("Executable found: %s\n", fullPath);
                printf("Permission: Executable\n");

                free(pathCopy);
                return;
            }
            else {
                printf("File found but no execute permission: %s\n",
                       fullPath);
            }
        }

        directory = strtok(NULL, ":");
    }

    printf("Command '%s' not found in PATH.\n", command);

    free(pathCopy);
}

int main() {
    pid_t pid;
    int status;
    char command[MAX_CMD];

    printf("========================================\n");
    printf(" PROCESS SYNCHRONIZATION AND PATH\n");
    printf("========================================\n");

    /* Display PATH variable */
    printf("\n--- PATH Variable ---\n");

    char *path = getenv("PATH");

    if (path != NULL) {
        printf("PATH = %s\n", path);
    }
    else {
        printf("PATH variable is not available.\n");
    }

    /* Get command from user */
    printf("\nEnter a command to locate: ");
    scanf("%99s", command);

    printf("\n--- Command Resolution ---\n");
    findExecutable(command);

    /* Create child process */
    printf("\n--- Creating Child Process ---\n");

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {

        /* Child process */
        printf("\nChild Process Started\n");
        printf("Child PID: %d\n", getpid());

        printf("Child is executing 'ls -l'...\n\n");

        execlp("ls", "ls", "-l", NULL);

        /* Executes only if exec fails */
        perror("Execution failed");
        exit(1);
    }

    else {

        /* Parent process */
        printf("\nParent Process\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID : %d\n", pid);

        printf("Parent waiting for child...\n");

        /* Wait for specific child */
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return 1;
        }

        printf("\n--- Child Process Status ---\n");

        if (WIFEXITED(status)) {
            printf("Child terminated normally.\n");
            printf("Exit status: %d\n",
                   WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) {
            printf("Child terminated by a signal.\n");
        }
        else {
            printf("Child status changed.\n");
        }
    }

    printf("\n========================================\n");
    printf("Program completed successfully.\n");
    printf("========================================\n");

    return 0;
}
