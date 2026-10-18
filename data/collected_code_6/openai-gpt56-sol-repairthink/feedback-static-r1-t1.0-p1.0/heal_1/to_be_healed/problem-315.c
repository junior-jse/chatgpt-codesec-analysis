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

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result, best_start, best_length);
    result[best_length] = '\0';

    return best_length;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[1024];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[1024];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        int ch;

        do {
            /* Possible weaknesses found:
             * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
             */
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