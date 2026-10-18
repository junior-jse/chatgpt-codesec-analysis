#include <stdio.h>
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

void reverse_words(char *str, size_t len) {
    if (str == NULL || len == 0) {
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
    size_t buffer_size = 256;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return 1;
    }

    if (fgets(buffer, buffer_size, stdin) == NULL) {
        free(buffer);
        return 1;
    }

    size_t length = 0;
    while (length < buffer_size && buffer[length] != '\0') {
        length++;
    }

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
        length--;
    }

    reverse_words(buffer, length);

    printf("%s\n", buffer);

    free(buffer);
    return 0;
}