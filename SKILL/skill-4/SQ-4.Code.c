#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 100
#define MAX_ARGS 50

/* Token types */
typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_SEMICOLON
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

/* Command structure */
typedef struct {
    char *args[MAX_ARGS];
    int arg_count;
    char *input_file;
    char *output_file;
} Command;

/* Get token type as string */
const char *tokenTypeName(TokenType type)
{
    switch (type) {
        case TOKEN_WORD:         return "WORD";
        case TOKEN_PIPE:         return "PIPE";
        case TOKEN_REDIRECT_IN:  return "REDIRECT_IN";
        case TOKEN_REDIRECT_OUT: return "REDIRECT_OUT";
        case TOKEN_SEMICOLON:    return "SEMICOLON";
        default:                 return "UNKNOWN";
    }
}

/* Check if character is a delimiter */
int isDelimiter(char c)
{
    return c == '|' || c == '<' || c == '>' || c == ';';
}

/* ---------------- TOKENIZER ---------------- */

int tokenize(char *input, Token tokens[])
{
    int count = 0;
    int i = 0;

    while (input[i] != '\0') {

        /* Ignore whitespace */
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        /* Check maximum token limit */
        if (count >= MAX_TOKENS) {
            printf("Error: Too many tokens.\n");
            return -1;
        }

        /* Delimiters */
        if (input[i] == '|') {
            tokens[count].type = TOKEN_PIPE;
            strcpy(tokens[count].value, "|");
            count++;
            i++;
        }
        else if (input[i] == '<') {
            tokens[count].type = TOKEN_REDIRECT_IN;
            strcpy(tokens[count].value, "<");
            count++;
            i++;
        }
        else if (input[i] == '>') {
            tokens[count].type = TOKEN_REDIRECT_OUT;
            strcpy(tokens[count].value, ">");
            count++;
            i++;
        }
        else if (input[i] == ';') {
            tokens[count].type = TOKEN_SEMICOLON;
            strcpy(tokens[count].value, ";");
            count++;
            i++;
        }

        /* Word */
        else {
            int j = 0;

            while (input[i] != '\0' &&
                   !isspace((unsigned char)input[i]) &&
                   !isDelimiter(input[i])) {

                if (j < MAX_TOKEN_LEN - 1) {
                    tokens[count].value[j++] = input[i];
                }

                i++;
            }

            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_WORD;
            count++;
        }
    }

    return count;
}

/* Display tokens */
void printTokens(Token tokens[], int count)
{
    printf("\n----- TOKEN STREAM -----\n");

    for (int i = 0; i < count; i++) {
        printf("Token %d: %-15s Value: %s\n",
               i + 1,
               tokenTypeName(tokens[i].type),
               tokens[i].value);
    }
}

/* ---------------- PARSER ---------------- */

/*
   Validate token sequence.
   Examples of invalid syntax:
   ls |
   | ls
   ls >
   ls < file >
*/

int validateTokens(Token tokens[], int count)
{
    if (count == 0) {
        printf("\nEmpty command.\n");
        return 0;
    }

    /* Command cannot start with pipe or semicolon */
    if (tokens[0].type == TOKEN_PIPE ||
        tokens[0].type == TOKEN_SEMICOLON) {

        printf("\nSyntax Error: Command cannot start with '%s'\n",
               tokens[0].value);
        return 0;
    }

    for (int i = 0; i < count; i++) {

        /* Pipe must have commands on both sides */
        if (tokens[i].type == TOKEN_PIPE) {

            if (i == 0 || i == count - 1) {
                printf("\nSyntax Error: Invalid pipe placement.\n");
                return 0;
            }

            if (tokens[i - 1].type != TOKEN_WORD ||
                tokens[i + 1].type != TOKEN_WORD) {

                printf("\nSyntax Error: Pipe must connect commands.\n");
                return 0;
            }
        }

        /* Redirection must be followed by filename */
        if (tokens[i].type == TOKEN_REDIRECT_IN ||
            tokens[i].type == TOKEN_REDIRECT_OUT) {

            if (i == count - 1 ||
                tokens[i + 1].type != TOKEN_WORD) {

                printf("\nSyntax Error: Redirection '%s' needs a filename.\n",
                       tokens[i].value);
                return 0;
            }
        }

        /* Semicolon cannot be at the end */
        if (tokens[i].type == TOKEN_SEMICOLON) {

            if (i == count - 1) {
                printf("\nSyntax Error: Empty command after ';'\n");
                return 0;
            }
        }
    }

    printf("\nSyntax validation successful.\n");
    return 1;
}

/* ---------------- PARSE TREE ---------------- */

void printParseTree(Token tokens[], int count)
{
    printf("\n----- PARSE TREE -----\n");

    printf("COMMAND\n");

    for (int i = 0; i < count; i++) {

        if (tokens[i].type == TOKEN_PIPE) {
            printf("  |\n");
            printf("  PIPE\n");
        }
        else if (tokens[i].type == TOKEN_REDIRECT_IN) {
            printf("  |\n");
            printf("  INPUT REDIRECTION\n");
        }
        else if (tokens[i].type == TOKEN_REDIRECT_OUT) {
            printf("  |\n");
            printf("  OUTPUT REDIRECTION\n");
        }
        else if (tokens[i].type == TOKEN_SEMICOLON) {
            printf("  |\n");
            printf("  COMMAND SEPARATOR\n");
        }
        else {
            printf("  |\n");
            printf("  WORD: %s\n", tokens[i].value);
        }
    }
}

/* ---------------- EXECUTION STRUCTURE ---------------- */

int buildCommands(Token tokens[], int count, Command commands[])
{
    int cmdCount = 0;

    commands[0].arg_count = 0;
    commands[0].input_file = NULL;
    commands[0].output_file = NULL;

    cmdCount = 1;

    for (int i = 0; i < count; i++) {

        /* Normal argument */
        if (tokens[i].type == TOKEN_WORD) {

            /* Skip filename if it belongs to redirection */
            if (i > 0 &&
                (tokens[i - 1].type == TOKEN_REDIRECT_IN ||
                 tokens[i - 1].type == TOKEN_REDIRECT_OUT)) {

                if (tokens[i - 1].type == TOKEN_REDIRECT_IN) {
                    commands[cmdCount - 1].input_file =
                        strdup(tokens[i].value);
                }
                else {
                    commands[cmdCount - 1].output_file =
                        strdup(tokens[i].value);
                }

                continue;
            }

            if (commands[cmdCount - 1].arg_count < MAX_ARGS - 1) {

                commands[cmdCount - 1].args[
                    commands[cmdCount - 1].arg_count
                ] = strdup(tokens[i].value);

                commands[cmdCount - 1].arg_count++;
            }
        }

        /* New command after pipe */
        else if (tokens[i].type == TOKEN_PIPE) {

            commands[cmdCount].arg_count = 0;
            commands[cmdCount].input_file = NULL;
            commands[cmdCount].output_file = NULL;

            cmdCount++;
        }
    }

    return cmdCount;
}

/* Display execution structure */
void printExecutionStructure(Command commands[], int count)
{
    printf("\n----- EXECUTION STRUCTURE -----\n");

    for (int i = 0; i < count; i++) {

        printf("\nCommand %d:\n", i + 1);

        printf("Arguments: ");

        for (int j = 0; j < commands[i].arg_count; j++) {
            printf("%s ", commands[i].args[j]);
        }

        printf("\n");

        if (commands[i].input_file != NULL)
            printf("Input File : %s\n",
                   commands[i].input_file);

        if (commands[i].output_file != NULL)
            printf("Output File: %s\n",
                   commands[i].output_file);
    }
}

/* Free dynamically allocated memory */
void freeCommands(Command commands[], int count)
{
    for (int i = 0; i < count; i++) {

        for (int j = 0; j < commands[i].arg_count; j++) {
            free(commands[i].args[j]);
        }

        if (commands[i].input_file != NULL)
            free(commands[i].input_file);

        if (commands[i].output_file != NULL)
            free(commands[i].output_file);
    }
}

/* ---------------- MAIN ---------------- */

int main()
{
    char input[MAX_INPUT];

    Token tokens[MAX_TOKENS];
    Command commands[MAX_TOKENS];

    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    /* Remove newline */
    input[strcspn(input, "\n")] = '\0';

    /* Handle empty command */
    if (strlen(input) == 0) {
        printf("Empty command entered.\n");
        return 0;
    }

    /* Step 1: Tokenization */
    int tokenCount = tokenize(input, tokens);

    if (tokenCount < 0) {
        return 1;
    }

    if (tokenCount == 0) {
        printf("No tokens found.\n");
        return 0;
    }

    /* Step 2: Display tokens */
    printTokens(tokens, tokenCount);

    /* Step 3: Validate syntax */
    if (!validateTokens(tokens, tokenCount)) {
        return 1;
    }

    /* Step 4: Generate parse tree */
    printParseTree(tokens, tokenCount);

    /* Step 5: Build execution structure */
    int commandCount =
        buildCommands(tokens, tokenCount, commands);

    /* Step 6: Display execution structure */
    printExecutionStructure(commands, commandCount);

    /* Step 7: Release memory */
    freeCommands(commands, commandCount);

    printf("\nParsing completed successfully.\n");

    return 0;
}
