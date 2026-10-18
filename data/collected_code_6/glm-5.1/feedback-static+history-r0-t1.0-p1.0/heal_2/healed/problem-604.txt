#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverse_words(char *str) {
    if (str == NULL) {
        return;
    }

    size_t len = strnlen(str, 256);
    if (len == 0) {
        return;
    }

    reverse(str, str + len - 1);

    char *word_start = str;
    char *word_end = str;

    while (*word_end) {
        if (*word_end == ' ') {
            if (word_start != word_end) {
                reverse(word_start, word_end - 1);
            }
            word_end++;
            word_start = word_end;
        } else {
            word_end++;
        }
    }

    if (word_start != word_end) {
        reverse(word_start, word_end - 1);
    }
}

int main(void) {
    char *buffer = malloc(256);
    if (buffer == NULL) {
        return 1;
    }

    if (fgets(buffer, 256, stdin) == NULL) {
        free(buffer);
        return 1;
    }

    size_t length = strnlen(buffer, 256);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    reverse_words(buffer);

    printf("%s\n", buffer);

    free(buffer);

    return 0;
}