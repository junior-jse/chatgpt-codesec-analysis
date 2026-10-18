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

static void reverse_words(char *str)
{
    size_t length;
    char *word_start;
    char *current;

    if (str == NULL) {
        return;
    }

    length = strlen(str);
    if (length < 2U) {
        return;
    }

    reverse_range(str, str + length - 1U);

    current = str;
    while (*current != '\0') {
        while (*current == ' ') {
            ++current;
        }

        word_start = current;

        while (*current != '\0' && *current != ' ') {
            ++current;
        }

        if (word_start < current) {
            reverse_range(word_start, current - 1);
        }
    }
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0U;
    ssize_t length;

    length = getline(&line, &capacity, stdin);
    if (length < 0) {
        free(line);
        return EXIT_FAILURE;
    }

    if (length > 0 && line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }

    reverse_words(line);

    if (printf("%s\n", line) < 0) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}