#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define INITIAL_BUFFER_SIZE 128

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

char find_most_common_char(const char *str, int *count) {
    int char_count[UCHAR_MAX + 1] = {0};
    size_t i;
    int max_count = 0;
    char most_common = '\0';

    if (str == NULL || count == NULL) {
        if (count != NULL) {
            *count = 0;
        }
        return '\0';
    }

    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        char_count[c]++;
    }

    for (i = 0; i <= UCHAR_MAX; i++) {
        if (char_count[i] > max_count) {
            max_count = char_count[i];
            most_common = (char)i;
        }
    }

    *count = max_count;
    return most_common;
}

char *read_line(FILE *stream) {
    size_t capacity = INITIAL_BUFFER_SIZE;
    size_t length = 0;
    size_t chunk;
    char *grown;
    char *buffer = malloc(capacity);

    if (buffer == NULL) {
        return NULL;
    }

    while (fgets(buffer + length, (int)(capacity - length), stream) != NULL) {
        chunk = bounded_strlen(buffer + length, capacity - length);
        length += chunk;

        if (length + 1 < capacity) {
            return buffer;
        }

        if (capacity > SIZE_MAX / 2) {
            free(buffer);
            return NULL;
        }
        capacity *= 2;

        grown = realloc(buffer, capacity);
        if (grown == NULL) {
            free(buffer);
            return NULL;
        }
        buffer = grown;
    }

    if (length == 0) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

int main(void) {
    char *buffer;
    int count = 0;
    char result;

    printf("Enter a string: ");

    buffer = read_line(stdin);
    if (buffer == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0') {
        fprintf(stderr, "Empty string provided\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    result = find_most_common_char(buffer, &count);

    if (count > 0) {
        printf("Most common character: '%c' (appears %d times)\n", result, count);
    } else {
        printf("No characters found\n");
    }

    free(buffer);
    return EXIT_SUCCESS;
}