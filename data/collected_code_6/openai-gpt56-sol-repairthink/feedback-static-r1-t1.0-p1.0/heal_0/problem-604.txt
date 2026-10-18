#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void reverse_range(char *start, char *end)
{
    while (start < end) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
}

static void reverse_words(char *string)
{
    size_t length;
    char *word_start;
    char *cursor;

    if (string == NULL) {
        return;
    }

    length = strlen(string);
    if (length < 2U) {
        return;
    }

    reverse_range(string, string + length - 1U);

    cursor = string;
    while (*cursor != '\0') {
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' ||
               *cursor == '\r' || *cursor == '\f' || *cursor == '\v') {
            ++cursor;
        }

        word_start = cursor;

        while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' &&
               *cursor != '\n' && *cursor != '\r' && *cursor != '\f' &&
               *cursor != '\v') {
            ++cursor;
        }

        if (word_start < cursor) {
            reverse_range(word_start, cursor - 1);
        }
    }
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0U;
    size_t length = 0U;
    int character;
    int status = EXIT_SUCCESS;

    while ((character = getchar()) != '\n' && character != EOF) {
        if (length + 1U >= capacity) {
            size_t new_capacity = capacity == 0U ? 128U : capacity * 2U;
            char *new_line;

            if (new_capacity <= capacity) {
                status = EXIT_FAILURE;
                goto cleanup;
            }

            new_line = realloc(line, new_capacity);
            if (new_line == NULL) {
                status = EXIT_FAILURE;
                goto cleanup;
            }

            line = new_line;
            capacity = new_capacity;
        }

        line[length++] = (char)character;
    }

    if (ferror(stdin)) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    if (line == NULL) {
        line = malloc(1U);
        if (line == NULL) {
            status = EXIT_FAILURE;
            goto cleanup;
        }
    }

    line[length] = '\0';
    reverse_words(line);

    if (puts(line) == EOF) {
        status = EXIT_FAILURE;
    }

cleanup:
    free(line);
    return status;
}