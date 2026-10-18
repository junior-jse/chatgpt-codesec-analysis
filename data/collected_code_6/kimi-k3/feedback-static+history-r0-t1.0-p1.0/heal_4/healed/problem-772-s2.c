#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define BUFFER_SIZE 256

void remove_words_of_length(char *str, size_t k)
{
    if (str == NULL) {
        return;
    }

    size_t read_idx = 0;
    size_t write_idx = 0;
    size_t str_len = strnlen(str, BUFFER_SIZE);

    while (read_idx < str_len) {
        while (read_idx < str_len && isspace((unsigned char)str[read_idx])) {
            if (write_idx < BUFFER_SIZE - 1) {
                str[write_idx++] = str[read_idx++];
            } else {
                read_idx++;
            }
        }

        if (read_idx >= str_len) {
            break;
        }

        size_t word_start = read_idx;
        while (read_idx < str_len && !isspace((unsigned char)str[read_idx])) {
            read_idx++;
        }
        size_t word_len = read_idx - word_start;

        if (word_len != k) {
            if (write_idx > 0 && write_idx < str_len && !isspace((unsigned char)str[write_idx - 1])) {
                if (write_idx < BUFFER_SIZE - 1) {
                    str[write_idx++] = ' ';
                }
            }
            if (write_idx + word_len < BUFFER_SIZE) {
                memmove(&str[write_idx], &str[word_start], word_len);
                write_idx += word_len;
            } else {
                size_t remaining = BUFFER_SIZE - 1 - write_idx;
                if (remaining > 0) {
                    memmove(&str[write_idx], &str[word_start], remaining);
                    write_idx += remaining;
                }
            }
        }
    }

    str[write_idx] = '\0';
}

int main(void)
{
    char buffer[BUFFER_SIZE];
    size_t k = 0;

    printf("Enter a string: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t len = strnlen(buffer, BUFFER_SIZE);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == BUFFER_SIZE - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    printf("Enter word length to remove: ");
    if (scanf("%zu", &k) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    remove_words_of_length(buffer, k);

    printf("Result: %s\n", buffer);

    return EXIT_SUCCESS;
}