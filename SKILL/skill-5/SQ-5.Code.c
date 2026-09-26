#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char name[] = "Shashank";
    char single_quote[100];
    char double_quote[100];

    printf("========================================\n");
    printf("   SINGLE QUOTES AND DOUBLE QUOTES\n");
    printf("========================================\n");

    /* Single quote simulation */
    strcpy(single_quote, "$name is not expanded");

    printf("\n--- Single Quotes ---\n");
    printf("Input  : '$name is not expanded'\n");
    printf("Output : %s\n", single_quote);
    printf("Result : Variable is treated as literal text.\n");

    /* Double quote simulation */
    sprintf(double_quote, "%s is expanded", name);

    printf("\n--- Double Quotes ---\n");
    printf("Input  : \"$name is expanded\"\n");
    printf("Output : %s\n", double_quote);
    printf("Result : Variable value is inserted.\n");

    /* Preserve spaces */
    char message[] = "Hello World from Linux";

    printf("\n--- Preserve Spaces ---\n");
    printf("Double quoted string: \"%s\"\n", message);

    /* Store quoted strings */
    char quoted[100];

    strcpy(quoted, "This is a stored quoted string");

    printf("\n--- Store Quoted String ---\n");
    printf("Stored value: \"%s\"\n", quoted);

    /* Nested tokens */
    printf("\n--- Nested Tokens ---\n");
    printf("Example: \"He said 'Hello World'\"\n");

    /* Edge case testing */
    printf("\n--- Edge Case Testing ---\n");
    printf("Single quote: '$name'\n");
    printf("Double quote: \"%s\"\n", name);
    printf("Spaces: \"Hello    World\"\n");

    printf("\n========================================\n");
    printf("Program completed successfully.\n");
    printf("========================================\n");

    return 0;
}
