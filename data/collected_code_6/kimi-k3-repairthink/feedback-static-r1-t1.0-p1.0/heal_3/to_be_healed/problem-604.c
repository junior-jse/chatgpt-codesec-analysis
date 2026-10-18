#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
    if (str == NULL || *str == '\0') {
        return;
    }

    char *word_start = NULL;
    char *current = str;

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
        char *newline;

        if (length + 1 >= capacity) {
            size_t new_capacity = capacity * 2;
            char *resized = realloc(buffer, new_capacity);
            if (resized == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = resized;
            capacity = new_capacity;
        }

        if (fgets(buffer + length, (int)(capacity - length), stream) == NULL) {
            break;
        }
        got_data = 1;

        newline = strchr(buffer + length, '\n');
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        length += strlen(buffer + length);

        if (newline != NULL) {
            *newline = '\0';
            break;
        }

        if (length + 1 < capacity) {
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