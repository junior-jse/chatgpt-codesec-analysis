#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_words_of_length_k(char *str, int k) {
    if (str == NULL || k <= 0) {
        return;
    }

    size_t len = strlen(str);
    size_t read = 0;
    size_t write = 0;

    while (read < len) {
        while (read < len && isspace((unsigned char)str[read])) {
            read++;
        }

        if (read >= len) {
            break;
        }

        size_t word_start = read;

        while (read < len && !isspace((unsigned char)str[read])) {
            read++;
        }

        size_t word_len = read - word_start;

        if (word_len != (size_t)k) {
            if (write != 0) {
                str[write] = ' ';
                write++;
            }
            memmove(str + write, str + word_start, word_len);
            write += word_len;
        }
    }

    str[write] = '\0';
}

int main() {
    char str[] = "This is a test string for removing words";
    int k = 4;

    printf("Original: \"%s\"\n", str);
    remove_words_of_length_k(str, k);
    printf("Modified: \"%s\"\n", str);

    return 0;
}