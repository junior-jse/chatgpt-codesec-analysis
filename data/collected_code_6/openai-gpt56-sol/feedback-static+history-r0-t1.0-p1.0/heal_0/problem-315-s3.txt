#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *find_first_longest_even_word(const char *text)
{
    const char *best_start = NULL;
    size_t best_length = 0;
    const unsigned char *current = (const unsigned char *)text;

    if (text == NULL) {
        return NULL;
    }

    while (*current != '\0') {
        const unsigned char *start;
        size_t length;

        while (*current != '\0' && isspace(*current)) {
            ++current;
        }

        start = current;

        while (*current != '\0' && !isspace(*current)) {
            ++current;
        }

        length = (size_t)(current - start);

        if (length > 0 && length % 2 == 0 && length > best_length) {
            best_start = (const char *)start;
            best_length = length;
        }
    }

    if (best_start == NULL) {
        return NULL;
    }

    char *result = malloc(best_length + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, best_start, best_length);
    result[best_length] = '\0';

    return result;
}

int main(void)
{
    char input[4096];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        int character;

        do {
            character = getchar();
        } while (character != '\n' && character != EOF);

        fprintf(stderr, "Input is too long.\n");
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    char *word = find_first_longest_even_word(input);
    if (word == NULL) {
        puts("No even-length word found.");
        return EXIT_SUCCESS;
    }

    puts(word);
    free(word);

    return EXIT_SUCCESS;
}