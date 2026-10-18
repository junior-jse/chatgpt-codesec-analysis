#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>

void reverse_substring(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverse_words(char *str) {
    char *word_start;
    char *current;

    if (str == NULL || *str == '\0') {
        return;
    }

    word_start = NULL;
    current = str;

    while (*current != '\0') {
        if (word_start == NULL && !isspace((unsigned char)*current)) {
            word_start = current;
        }
        if (word_start != NULL && (isspace((unsigned char)*(current + 1)) || *(current + 1) == '\0')) {
            reverse_substring(word_start, current);
            word_start = NULL;
        }
        current++;
    }

    reverse_substring(str, current - 1);
}

char *read_line(FILE *stream) {
    size_t capacity = 256;
    size_t length = 0;
    int got_data = 0;
    char *buffer = malloc(capacity);

    if (buffer == NULL) {
        return NULL;
    }

    for (;;) {
        size_t space;
        size_t chunk;
        char *terminator;
        char *newline;

        if (length + 1 >= capacity) {
            size_t new_capacity;
            char *resized;

            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }
            new_capacity = capacity * 2;
            resized = realloc(buffer, new_capacity);
            if (resized == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = resized;
            capacity = new_capacity;
        }

        space = capacity - length;
        if (space > (size_t)INT_MAX) {
            space = (size_t)INT_MAX;
        }

        if (fgets(buffer + length, (int)space, stream) == NULL) {
            break;
        }
        got_data = 1;

        terminator = memchr(buffer + length, '\0', space);
        if (terminator == NULL) {
            free(buffer);
            return NULL;
        }
        chunk = (size_t)(terminator - (buffer + length));

        newline = memchr(buffer + length, '\n', chunk);
        length += chunk;

        if (newline != NULL) {
            *newline = '\0';
            break;
        }

        if (chunk + 1 < space) {
            break;
        }
    }

    if (!got_data) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

int main(void) {
    char *buffer = read_line(stdin);

    if (buffer == NULL) {
        return EXIT_FAILURE;
    }

    reverse_words(buffer);

    if (printf("%s\n", buffer) < 0) {
        free(buffer);
        return EXIT_FAILURE;
    }

    free(buffer);
    return EXIT_SUCCESS;
}