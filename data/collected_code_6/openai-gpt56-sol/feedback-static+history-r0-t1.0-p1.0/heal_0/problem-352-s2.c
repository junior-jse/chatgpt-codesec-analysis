#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

bool all_characters_unique(const char *string)
{
    bool seen[256] = { false };

    if (string == NULL) {
        return false;
    }

    for (size_t i = 0; string[i] != '\0'; ++i) {
        unsigned char character = (unsigned char)string[i];

        if (seen[character]) {
            return false;
        }

        seen[character] = true;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    printf("%s\n", all_characters_unique(argv[1]) ? "true" : "false");
    return 0;
}