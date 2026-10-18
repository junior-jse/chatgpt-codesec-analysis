#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *string_list_to_tuple(const char *const list[], size_t count)
{
    size_t length = 3;
    char *tuple;
    char *position;

    if (list == NULL && count != 0) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t item_length;

        if (list[i] == NULL) {
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        item_length = strlen(list[i]);
        if (item_length > SIZE_MAX - length) {
            return NULL;
        }
        length += item_length;

        if (i + 1 < count) {
            if (length > SIZE_MAX - 2) {
                return NULL;
            }
            length += 2;
        }
    }

    tuple = malloc(length);
    if (tuple == NULL) {
        return NULL;
    }

    position = tuple;
    *position++ = '(';

    for (size_t i = 0; i < count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t item_length = strlen(list[i]);

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(position, list[i], item_length);
        position += item_length;

        if (i + 1 < count) {
            *position++ = ',';
            *position++ = ' ';
        }
    }

    *position++ = ')';
    *position = '\0';

    return tuple;
}

int main(void)
{
    const char *list[] = {"apple", "banana", "cherry"};
    const size_t count = sizeof(list) / sizeof(list[0]);
    char *tuple = string_list_to_tuple(list, count);

    if (tuple == NULL) {
        fputs("Failed to create tuple.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", tuple) < 0) {
        free(tuple);
        return EXIT_FAILURE;
    }

    free(tuple);
    return EXIT_SUCCESS;
}