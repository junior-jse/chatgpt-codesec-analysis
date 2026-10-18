#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_list(char **list, size_t count)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(list[i]);
    }

    free(list);
}

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1);
    return copy;
}

static int tuple_to_list(const char *const tuple[], size_t tuple_count,
                         const char *separator, char ***result,
                         size_t *result_count)
{
    char **list;
    size_t count;

    if (result == NULL || result_count == NULL || separator == NULL ||
        (tuple == NULL && tuple_count != 0) ||
        tuple_count > SIZE_MAX / 2 ||
        tuple_count * 2 > SIZE_MAX / sizeof(*list)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;
    count = tuple_count * 2;

    if (count == 0) {
        return 0;
    }

    list = calloc(count, sizeof(*list));
    if (list == NULL) {
        return -1;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (tuple[i] == NULL) {
            free_list(list, i * 2);
            return -1;
        }

        list[i * 2] = duplicate_string(tuple[i]);
        if (list[i * 2] == NULL) {
            free_list(list, i * 2);
            return -1;
        }

        list[i * 2 + 1] = duplicate_string(separator);
        if (list[i * 2 + 1] == NULL) {
            free_list(list, i * 2 + 1);
            return -1;
        }
    }

    *result = list;
    *result_count = count;
    return 0;
}

int main(void)
{
    const char *tuple[] = {"apple", "banana", "cherry"};
    const char *separator = "added";
    char **list = NULL;
    size_t list_count = 0;
    size_t tuple_count = sizeof(tuple) / sizeof(tuple[0]);

    if (tuple_to_list(tuple, tuple_count, separator,
                      &list, &list_count) != 0) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list_count; ++i) {
        if (printf("%s\n", list[i]) < 0) {
            free_list(list, list_count);
            return EXIT_FAILURE;
        }
    }

    free_list(list, list_count);
    return EXIT_SUCCESS;
}