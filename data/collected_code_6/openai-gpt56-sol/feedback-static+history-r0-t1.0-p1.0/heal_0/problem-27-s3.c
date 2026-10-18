#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void remove_digits(char *string)
{
    char *read_pos = string;
    char *write_pos = string;

    while (*read_pos != '\0') {
        unsigned char character = (unsigned char)*read_pos;

        if (!isdigit(character)) {
            *write_pos++ = *read_pos;
        }

        ++read_pos;
    }

    *write_pos = '\0';
}

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);

    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

int main(int argc, char *argv[])
{
    char **strings;
    size_t count;
    size_t i;
    int status = EXIT_SUCCESS;

    if (argc < 1) {
        return EXIT_FAILURE;
    }

    count = (size_t)(argc - 1);

    if (count == 0U) {
        return EXIT_SUCCESS;
    }

    if (count > SIZE_MAX / sizeof(*strings)) {
        return EXIT_FAILURE;
    }

    strings = calloc(count, sizeof(*strings));
    if (strings == NULL) {
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        strings[i] = duplicate_string(argv[i + 1U]);
        if (strings[i] == NULL) {
            status = EXIT_FAILURE;
            break;
        }

        remove_digits(strings[i]);
    }

    if (status == EXIT_SUCCESS) {
        for (i = 0U; i < count; ++i) {
            if (puts(strings[i]) == EOF) {
                status = EXIT_FAILURE;
                break;
            }
        }
    }

    for (i = 0U; i < count; ++i) {
        free(strings[i]);
    }

    free(strings);
    return status;
}