#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool all_characters_unique(const char *string)
{
    bool seen[256] = { false };

    if (string == NULL) {
        return false;
    }

    for (const unsigned char *p = (const unsigned char *)string; *p != '\0'; ++p) {
        if (seen[*p]) {
            return false;
        }
        seen[*p] = true;
    }

    return true;
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0U;
    ssize_t length = getline(&line, &capacity, stdin);

    if (length < 0) {
        free(line);
        if (ferror(stdin)) {
            fputs("Failed to read input.\n", stderr);
            return EXIT_FAILURE;
        }
        fputs("No input provided.\n", stderr);
        return EXIT_FAILURE;
    }

    if (length > 0 && line[length - 1] == '\n') {
        line[--length] = '\0';
    }
    if (length > 0 && line[length - 1] == '\r') {
        line[length - 1] = '\0';
    }

    puts(all_characters_unique(line)
             ? "All characters are unique."
             : "Duplicate characters found.");

    free(line);
    return EXIT_SUCCESS;
}