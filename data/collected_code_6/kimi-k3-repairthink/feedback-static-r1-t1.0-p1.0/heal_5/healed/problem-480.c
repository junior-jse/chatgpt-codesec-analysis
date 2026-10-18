#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

char find_max_occurrence(const char *str, size_t *max_count)
{
    size_t counts[UCHAR_MAX + 1] = {0};
    size_t i;
    unsigned char max_char = 0;
    size_t max = 0;

    if (str == NULL || *str == '\0') {
        if (max_count != NULL) {
            *max_count = 0;
        }
        return '\0';
    }

    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        counts[c]++;
        if (counts[c] > max) {
            max = counts[c];
            max_char = c;
        }
    }

    if (max_count != NULL) {
        *max_count = max;
    }

    return (char)max_char;
}

static char *read_line(FILE *stream)
{
    size_t capacity = 128;
    size_t length = 0;
    char *buffer;
    int ch;

    if (stream == NULL) {
        return NULL;
    }

    buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    while ((ch = fgetc(stream)) != EOF && ch != '\n') {
        if (length >= capacity - 1) {
            size_t new_capacity;
            char *new_buffer;

            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }

            new_capacity = capacity * 2;

            new_buffer = realloc(buffer, new_capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }

            buffer = new_buffer;
            capacity = new_capacity;
        }

        buffer[length] = (char)ch;
        length++;
    }

    if (ferror(stream)) {
        free(buffer);
        return NULL;
    }

    if (ch == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void)
{
    char *buffer;
    size_t count = 0;
    char result;

    printf("Enter a string: ");
    fflush(stdout);

    buffer = read_line(stdin);
    if (buffer == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    result = find_max_occurrence(buffer, &count);

    if (count > 0) {
        printf("Maximum occurring character: '%c' (appears %zu times)\n", result, count);
    } else {
        printf("String is empty\n");
    }

    free(buffer);
    return 0;
}