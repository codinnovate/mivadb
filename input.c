#include "input.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Remove a trailing newline from a string read by fgets. */
static void trimNewline(char *text) {
    size_t len = strlen(text);
    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }
}

/* Read a non-empty line safely from standard input. */
void readLine(const char *prompt, char *buffer, size_t size) {
    for (;;) {
        printf("%s", prompt);
        if (fgets(buffer, (int) size, stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }

        trimNewline(buffer);
        if (strlen(buffer) == 0) {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }
        break;
    }
}

/* Read and validate integer input. */
int readInt(const char *prompt) {
    char input[128];
    char *endPtr = NULL;
    long value;

    for (;;) {
        readLine(prompt, input, sizeof(input));
        value = strtol(input, &endPtr, 10);
        if (endPtr == input || *endPtr != '\0') {
            printf("Invalid number. Please enter an integer.\n");
            continue;
        }
        return (int) value;
    }
}

/* Read and validate floating-point input. */
float readFloat(const char *prompt) {
    char input[128];
    char *endPtr = NULL;
    float value;

    for (;;) {
        readLine(prompt, input, sizeof(input));
        value = strtof(input, &endPtr);
        if (endPtr == input || *endPtr != '\0') {
            printf("Invalid number. Please enter a numeric value.\n");
            continue;
        }
        return value;
    }
}

