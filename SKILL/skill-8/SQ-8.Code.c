#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 500

/* ---------- Built-in command functions ---------- */

void builtin_cd(char *args) {
    if (args == NULL) {
        printf("cd: missing directory\n");
        return;
    }

    if (chdir(args) != 0) {
        perror("cd");
    }
}

void builtin_pwd() {
    char directory[MAX];

    if (getcwd(directory, sizeof(directory)) != NULL) {
        printf("%s\n", directory);
    } else {
        perror("pwd");
    }
}

void builtin_echo(char *args) {
    if (args != NULL)
        printf("%s\n", args);
    else
        printf("\n");
}

/* ---------- Variable expansion ---------- */

void expandVariables(char *input, char *output) {
    int i = 0;
    int j = 0;

    while (input[i] != '\0') {

        if (input[i] == '$') {

            i++;

            char variable[100];
            int k = 0;

            /* Read variable name */
            while ((input[i] >= 'A' && input[i] <= 'Z') ||
                   (input[i] >= 'a' && input[i] <= 'z') ||
                   (input[i] >= '0' && input[i] <= '9') ||
                   input[i] == '_') {

                variable[k++] = input[i++];
            }

            variable[k] = '\0';

            /* Get environment variable */
            char *value = getenv(variable);

            if (value != NULL) {
                int m = 0;

                while (value[m] != '\0') {
                    output[j++] = value[m++];
                }
            }
            else {
                /* Undefined variable */
                printf("Warning: %s is undefined\n", variable);
            }
        }
        else {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
}

/* ---------- Built-in dispatch table ---------- */

struct Builtin {
    char name[20];
    int id;
};

struct Builtin dispatchTable[] = {
    {"cd", 1},
    {"pwd", 2},
    {"echo", 3},
    {"exit", 4}
};

int builtinCount = 4;

/* ---------- Find built-in ---------- */

int findBuiltin(char *command) {
    int i;

    for (i = 0; i < builtinCount; i++) {
        if (strcmp(command, dispatchTable[i].name) == 0) {
            return dispatchTable[i].id;
        }
    }

    return 0;
}

/* ---------- Main ---------- */

int main() {

    char input[MAX];
    char expanded[MAX];

    printf("========================================\n");
    printf(" VARIABLE EXPANSION & BUILT-IN COMMANDS\n");
    printf("========================================\n");

    while (1) {

        printf("\nmini-shell> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        /* Variable expansion */
        expandVariables(input, expanded);

        /* Display expanded command */
        printf("Expanded: %s\n", expanded);

        /* Separate command and arguments */
        char *command = strtok(expanded, " ");
        char *arguments = strtok(NULL, "");

        if (command == NULL)
            continue;

        /* Find built-in command */
        int id = findBuiltin(command);

        switch (id) {

            case 1:
                /* cd */
                builtin_cd(arguments);
                break;

            case 2:
                /* pwd */
                builtin_pwd();
                break;

            case 3:
                /* echo */
                builtin_echo(arguments);
                break;

            case 4:
                /* exit */
                printf("Exiting shell...\n");
                return 0;

            default:
                printf("Invalid command: %s\n", command);
                printf("Available commands: cd, pwd, echo, exit\n");
        }
    }

    return 0;
}
