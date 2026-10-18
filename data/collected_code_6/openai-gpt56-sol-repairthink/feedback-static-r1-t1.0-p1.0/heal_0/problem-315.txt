#include <ctype.h>
#include <stdio.h>
#include <string.h>

size_t find_first_max_even_word(const char *text, char *result, size_t result_size)
{
    const char *best_start = NULL;
    size_t best_length = 0;

    if (text == NULL || result == NULL || result_size == 0) {
        return 0;
    }

    result[0] = '\0';

    while (*text != '\0') {
        const char *word_start;
        size_t word_length = 0;

        while (*text != '\0' && isspace((unsigned char)*text)) {
            ++text;
        }

        word_start = text;

        while (*text != '\0' && !isspace((unsigned char)*text)) {
            ++word_length;
            ++text;
        }

        if (word_length % 2 == 0 && word_length > best_length) {
            best_start = word_start;
            best_length = word_length;
        }
    }

    if (best_start == NULL || best_length >= result_size) {
        return 0;
    }

    memcpy(result, best_start, best_length);
    result[best_length] = '\0';

    return best_length;
}

int main(void)
{
    char input[1024];
    char result[1024];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        int ch;

        do {
            ch = getchar();
        } while (ch != '\n' && ch != EOF);

        if (ferror(stdin)) {
            return 1;
        }
    }

    input[strcspn(input, "\r\n")] = '\0';

    if (find_first_max_even_word(input, result, sizeof(result)) > 0) {
        if (printf("%s\n", result) < 0) {
            return 1;
        }
    }

    return 0;
}