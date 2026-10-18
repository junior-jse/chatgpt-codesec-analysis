#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>

int find_first_max_even_word_length(const char *str) {
    int max_len = 0;
    int curr_len = 0;
    int in_word = 0;
    const char *p;

    if (str == NULL) {
        return 0;
    }

    p = str;

    while (*p != '\0') {
        if (isspace((unsigned char)*p)) {
            if (in_word) {
                if (curr_len % 2 == 0 && curr_len > max_len) {
                    max_len = curr_len;
                }
                in_word = 0;
                curr_len = 0;
            }
        } else {
            in_word = 1;
            curr_len++;
        }
        p++;
    }

    if (in_word && curr_len % 2 == 0 && curr_len > max_len) {
        max_len = curr_len;
    }

    return max_len;
}

int main(void) {
    char *input = NULL;
    size_t buffer_size = 0;
    ssize_t bytes_read;
    int result;

    printf("Enter a string: ");

    bytes_read = getline(&input, &buffer_size, stdin);
    if (bytes_read < 0) {
        fprintf(stderr, "Error reading input\n");
        free(input);
        return EXIT_FAILURE;
    }

    if (bytes_read > 0 && input[bytes_read - 1] == '\n') {
        input[bytes_read - 1] = '\0';
    }

    result = find_first_max_even_word_length(input);
    printf("Maximum length of even word: %d\n", result);

    free(input);
    return EXIT_SUCCESS;
}