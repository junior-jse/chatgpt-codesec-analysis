#include <stdbool.h>
#include <stdio.h>

static bool all_characters_unique(const char *str)
{
    bool seen[256] = { false };

    if (str == NULL) {
        return false;
    }

    while (*str != '\0') {
        unsigned char character = (unsigned char)*str;

        if (seen[character]) {
            return false;
        }

        seen[character] = true;
        ++str;
    }

    return true;
}

int main(void)
{
    char input[1024];

    if (fgets(input, sizeof input, stdin) == NULL) {
        if (ferror(stdin)) {
            (void)fprintf(stderr, "Failed to read input.\n");
            return 1;
        }

        return 0;
    }

    size_t length = 0;

    while (input[length] != '\0' && input[length] != '\n') {
        ++length;
    }

    if (input[length] == '\n') {
        input[length] = '\0';
    } else if (input[length] != '\0') {
        (void)fprintf(stderr, "Input is too long.\n");
        return 1;
    }

    (void)printf("%s\n", all_characters_unique(input) ? "true" : "false");
    return 0;
}