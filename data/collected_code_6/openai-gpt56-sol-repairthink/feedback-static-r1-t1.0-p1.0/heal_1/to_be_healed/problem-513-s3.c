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

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static int tuple_to_list(const char *const tuple[], size_t tuple_count,
                         const char *added_string, char ***result,
                         size_t *result_count)
{
    char **list;
    size_t list_count;
    size_t initialized = 0U;

    if (added_string == NULL || result == NULL || result_count == NULL ||
        (tuple == NULL && tuple_count != 0U)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0U;

    if (tuple_count > SIZE_MAX / 2U) {
        return -1;
    }

    list_count = tuple_count * 2U;

    if (list_count == 0U) {
        return 0;
    }

    if (list_count > SIZE_MAX / sizeof(*list)) {
        return -1;
    }

    list = calloc(list_count, sizeof(*list));
    if (list == NULL) {
        return -1;
    }

    for (size_t i = 0U; i < tuple_count; ++i) {
        if (tuple[i] == NULL) {
            free_list(list, initialized);
            return -1;
        }

        list[initialized] = duplicate_string(tuple[i]);
        if (list[initialized] == NULL) {
            free_list(list, initialized);
            return -1;
        }
        ++initialized;

        list[initialized] = duplicate_string(added_string);
        if (list[initialized] == NULL) {
            free_list(list, initialized);
            return -1;
        }
        ++initialized;
    }

    *result = list;
    *result_count = list_count;
    return 0;
}

int main(void)
{
    const char *const tuple[] = {"one", "two", "three"};
    const size_t tuple_count = sizeof(tuple) / sizeof(tuple[0]);
    const char *added_string = "added";
    char **list = NULL;
    size_t list_count = 0U;

    if (tuple_to_list(tuple, tuple_count, added_string,
                      &list, &list_count) != 0) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < list_count; ++i) {
        if (printf("%s\n", list[i]) < 0) {
            free_list(list, list_count);
            return EXIT_FAILURE;
        }
    }

    free_list(list, list_count);
    return EXIT_SUCCESS;
}