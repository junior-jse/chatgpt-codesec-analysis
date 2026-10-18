#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_words_of_length_k(char *str, int k) {
    if (str == NULL || k <= 0) {
        return;
    }

    char *read = str;
    char *write = str;
    int len = (int)strlen(str);

    while (*read) {
        while (*read && isspace((unsigned char)*read)) {
            read++;
        }

        if (!*read) {
            break;
        }

        char *word_start = read;

        while (*read && !isspace((unsigned char)*read)) {
            read++;
        }

        int word_len = (int)(read - word_start);

        if (word_len != k) {
            if (write != str) {
                *write = ' ';
                write++;
            }
            memmove(write, word_start, word_len);
            write += word_len;
        }
    }

    *write = '\0';
}

int main() {
    char str[] = "This is a test string for removing words";
    int k = 4;

    printf("Original: \"%s\"\n", str);
    remove_words_of_length_k(str, k);
    printf("Modified: \"%s\"\n", str);

    return 0;
}