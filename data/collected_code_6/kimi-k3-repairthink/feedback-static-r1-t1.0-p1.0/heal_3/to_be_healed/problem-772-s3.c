#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

static size_t bounded_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    if (str == NULL) {
        return 0;
    }
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

char *remove_k_length_words(const char *str, size_t k, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return NULL;
    }

    size_t len = bounded_strlen(str, max_len);
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;

    while (i < len) {
        while (i < len && isspace((unsigned char)str[i])) {
            i++;
        }

        if (i >= len) {
            break;
        }

        size_t word_start = i;
        while (i < len && !isspace((unsigned char)str[i])) {
            i++;
        }

        size_t word_len = i - word_start;

        if (word_len != k) {
            size_t needed = word_len + (j > 0 ? 1u : 0u);
            if (j <= len && needed <= len - j) {
                if (j > 0) {
                    result[j++] = ' ';
                }
                for (size_t m = 0; m < word_len && j < len; m++) {
                    result[j++] = str[word_start + m];
                }
            }
        }
    }

    result[j] = '\0';

    char *final_result = (char *)realloc(result, j + 1);
    if (final_result == NULL) {
        return result;
    }

    return final_result;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[MAX_INPUT_SIZE];
    size_t k = 0;

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    input[MAX_INPUT_SIZE - 1] = '\0';

    size_t input_len = bounded_strlen(input, sizeof(input));
    if (input_len >= sizeof(input) - 1 && input[sizeof(input) - 2] != '\n') {
        int ch;
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }

    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        /* Possible weaknesses found:
         *  Variable 'input_len' is assigned a value that is never used. [unreadVariable]
         */
        input_len--;
    }

    printf("Enter the word length to remove: ");
    if (scanf("%zu", &k) != 1) {
        fprintf(stderr, "Invalid input for k\n");
        return EXIT_FAILURE;
    }

    char *result = remove_k_length_words(input, k, sizeof(input));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Result: \"%s\"\n", result);

    free(result);
    result = NULL;

    return EXIT_SUCCESS;
}