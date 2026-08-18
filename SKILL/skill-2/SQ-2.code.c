#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define BUFFER_SIZE 100

// Function to enable character-by-character input
void enableRawMode(struct termios *old)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, old);
    raw = *old;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Function to restore normal terminal mode
void disableRawMode(struct termios *old)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, old);
}

int main()
{
    char buffer[BUFFER_SIZE];
    int index;
    char ch;
    struct termios old;

    printf("===== SIMPLE COMMAND SHELL =====\n");
    printf("Type a command and press Enter.\n");
    printf("Type 'exit' to quit.\n\n");

    while (1)
    {
        index = 0;

        printf("myshell> ");
        fflush(stdout);

        // Enable keyboard input character by character
        enableRawMode(&old);

        while (1)
        {
            ch = getchar();

            // Handle Enter key
            if (ch == '\n' || ch == '\r')
            {
                buffer[index] = '\0';
                printf("\n");
                break;
            }

            // Handle Backspace
            else if (ch == 127 || ch == '\b')
            {
                if (index > 0)
                {
                    index--;
                    printf("\b \b");
                    fflush(stdout);
                }
            }

            // Handle normal characters
            else if (ch >= 32 && ch <= 126)
            {
                if (index < BUFFER_SIZE - 1)
                {
                    buffer[index] = ch;
                    index++;

                    putchar(ch);
                    fflush(stdout);
                }
            }
        }

        // Restore normal terminal mode
        disableRawMode(&old);

        // Handle exit command
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        // Handle empty input
        if (strlen(buffer) == 0)
        {
            continue;
        }

        // Display entered command
        printf("You entered: %s\n", buffer);
    }

    return 0;
}
