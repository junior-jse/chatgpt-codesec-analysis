#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_with_length(const char* input, int k) {
    if (input == NULL || k <= 0) {
        return NULL;
    }

    size_t len = strlen(input);
    char* result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t out = 0;
    const char* p = input;

    while (*p) {
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }

        const char* word_start = p;
        while (*p && !isspace((unsigned char)*p)) {
            p++;
        }
        size_t word_len = p - word_start;

        if (word_len != (size_t)k) {
            if (out > 0) {
                result[out++] = ' ';
            }
            memcpy(result + out, word_start, word_len);
            out += word_len;
        }
    }

    result[out] = '\0';

    char* final_result = realloc(result, out + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }

    return final_result;
}

int main() {
    char input[1000];
    int k;

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }

    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        return 1;
    }

    char* result = remove_words_with_length(input, k);
    if (result == NULL) {
        printf("Error processing string\n");
        return 1;
    }

    printf("Result: %s\n", result);
    free(result);
    return 0;
}