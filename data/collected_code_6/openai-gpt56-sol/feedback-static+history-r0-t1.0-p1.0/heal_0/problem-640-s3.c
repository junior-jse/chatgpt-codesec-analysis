#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_parenthesized(char *str)
{
    size_t read_pos = 0;
    size_t write_pos = 0;
    size_t depth = 0;

    if (str == NULL) {
        return -1;
    }

    while (str[read_pos] != '\0') {
        if (str[read_pos] == '(') {
            ++depth;
        } else if (str[read_pos] == ')') {
            if (depth == 0) {
                return -1;
            }
            --depth;
        } else if (depth == 0) {
            str[write_pos++] = str[read_pos];
        }
        ++read_pos;
    }

    if (depth != 0) {
        return -1;
    }

    str[write_pos] = '\0';
    return 0;
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    length = getline(&line, &capacity, stdin);
    if (length < 0) {
        free(line);
        return EXIT_FAILURE;
    }

    if (length > 0 && line[(size_t)length - 1] == '\n') {
        line[(size_t)length - 1] = '\0';
    }

    if (remove_parenthesized(line) != 0) {
        free(line);
        return EXIT_FAILURE;
    }

    if (puts(line) == EOF) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}