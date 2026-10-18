#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_k_length_words(const char *str, size_t k) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    int prev_word_removed = 0;

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
            if (j > 0 && !prev_word_removed) {
                result[j++] = ' ';
            } else if (j > 0 && prev_word_removed) {
                result[j++] = ' ';
            }
            memcpy(result + j, str + word_start, word_len);
            j += word_len;
            prev_word_removed = 0;
        } else {
            prev_word_removed = 1;
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
    char input[1024];
    size_t k;

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    printf("Enter the word length to remove: ");
    if (scanf("%zu", &k) != 1) {
        fprintf(stderr, "Invalid input for k\n");
        return EXIT_FAILURE;
    }

    char *result = remove_k_length_words(input, k);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Result: \"%s\"\n", result);

    free(result);
    result = NULL;

    return EXIT_SUCCESS;
}