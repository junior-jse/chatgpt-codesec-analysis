#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

char* remove_words_of_length_k(const char* str, int k) {
    if (str == NULL || k < 0) {
        return NULL;
    }

    size_t max_len = 1048576;
    size_t len = strnlen(str, max_len);
    if (len == max_len && str[max_len] != '\0') {
        return NULL;
    }

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t res_idx = 0;
    size_t i = 0;

    while (i < len) {
        if (isalpha((unsigned char)str[i])) {
            size_t word_start = i;
            size_t word_len = 0;

            while (i < len && isalpha((unsigned char)str[i])) {
                word_len++;
                i++;
            }

            if (word_len != (size_t)k) {
                for (size_t j = word_start; j < word_start + word_len; j++) {
                    result[res_idx++] = str[j];
                }
            }
        } else {
            result[res_idx++] = str[i++];
        }
    }

    result[res_idx] = '\0';
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <string> <k>\n", argv[0]);
        return 1;
    }

    const char* input = argv[1];
    char* endptr;
    errno = 0;
    long k_long = strtol(argv[2], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || k_long < 0 || k_long > INT_MAX) {
        fprintf(stderr, "k must be a valid non-negative integer.\n");
        return 1;
    }

    int k = (int)k_long;

    char* output = remove_words_of_length_k(input, k);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed or invalid input.\n");
        return 1;
    }

    printf("%s\n", output);

    free(output);
    output = NULL;

    return 0;
}