#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_range(char *str, size_t start, size_t end, size_t size) {
    if (str == NULL || start >= size || end >= size) {
        return;
    }
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void reverse_words(char *str, size_t size) {
    if (str == NULL || size == 0) {
        return;
    }

    str[size - 1] = '\0';

    size_t len = strnlen(str, size);

    if (len == 0) {
        return;
    }

    reverse_range(str, 0, len - 1, size);

    size_t word_start = 0;
    size_t i = 0;

    while (i <= len) {
        if (str[i] == ' ' || str[i] == '\0') {
            if (i > word_start) {
                reverse_range(str, word_start, i - 1, size);
            }
            word_start = i + 1;
        }
        i++;
    }
}

int main(void) {
    size_t capacity = 256;
    char *buffer = malloc(capacity);
    if (buffer == NULL) {
        return EXIT_FAILURE;
    }
    buffer[0] = '\0';
    size_t len = 0;

    while (fgets(buffer + len, capacity - len, stdin) != NULL) {
        len = strnlen(buffer, capacity);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
            break;
        }
        if (len + 1 >= capacity) {
            capacity *= 2;
            char *tmp = realloc(buffer, capacity);
            if (tmp == NULL) {
                free(buffer);
                return EXIT_FAILURE;
            }
            buffer = tmp;
        }
    }

    if (len == 0 && ferror(stdin)) {
        free(buffer);
        return EXIT_FAILURE;
    }

    reverse_words(buffer, capacity);

    printf("%s\n", buffer);

    free(buffer);

    return EXIT_SUCCESS;
}