#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void remove_words_of_length(char *str, size_t k, size_t max_len)
{
    if (str == NULL || max_len == 0) {
        return;
    }

    size_t read_idx = 0;
    size_t write_idx = 0;
    size_t str_len = strnlen(str, max_len);

    while (read_idx < str_len) {
        while (read_idx < str_len && isspace((unsigned char)str[read_idx])) {
            str[write_idx++] = str[read_idx++];
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
                str[write_idx++] = ' ';
            }
            memmove(&str[write_idx], &str[word_start], word_len);
            write_idx += word_len;
        }
    }

    if (write_idx < max_len) {
        str[write_idx] = '\0';
    } else {
        str[max_len - 1] = '\0';
    }
}

int main(void)
{
    char buffer[256];
    size_t k;

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t len = strnlen(buffer, sizeof(buffer));
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == sizeof(buffer) - 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        fprintf(stderr, "Warning: input was truncated to fit buffer\n");
    }

    printf("Enter word length to remove: ");
    if (scanf("%zu", &k) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    remove_words_of_length(buffer, k, sizeof(buffer));

    printf("Result: %s\n", buffer);

    return EXIT_SUCCESS;
}