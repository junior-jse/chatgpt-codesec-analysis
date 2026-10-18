#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_words_of_length_k(char *str, size_t buffer_size, int k) {
    if (str == NULL || buffer_size == 0 || k <= 0) {
        return;
    }

    size_t len = strnlen(str, buffer_size);
    size_t read_idx = 0;
    size_t write_idx = 0;
    int is_first_word = 1;

    while (read_idx < len) {
        while (read_idx < len && isspace((unsigned char)str[read_idx])) {
            read_idx++;
        }

        if (read_idx >= len) {
            break;
        }

        size_t word_start = read_idx;

        while (read_idx < len && !isspace((unsigned char)str[read_idx])) {
            read_idx++;
        }

        size_t word_len = read_idx - word_start;

        if (word_len != (size_t)k) {
            if (!is_first_word) {
                str[write_idx++] = ' ';
            }
            memmove(str + write_idx, str + word_start, word_len);
            write_idx += word_len;
            is_first_word = 0;
        }
    }

    if (write_idx < buffer_size) {
        str[write_idx] = '\0';
    } else {
        str[buffer_size - 1] = '\0';
    }
}

int main() {
    char str[] = "This is a test string for removing words";
    int k = 4;

    printf("Original: \"%s\"\n", str);
    remove_words_of_length_k(str, sizeof(str), k);
    printf("Modified: \"%s\"\n", str);

    return 0;
}