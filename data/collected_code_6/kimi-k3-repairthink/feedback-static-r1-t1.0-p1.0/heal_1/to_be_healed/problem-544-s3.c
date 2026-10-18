#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleList;

char *flatten_tuple_list(const TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return NULL;
    }

    size_t total_len = 0;
    size_t i, j;

    for (i = 0; i < list->count; i++) {
        for (j = 0; j < list->tuples[i].count; j++) {
            if (list->tuples[i].items[j] != NULL) {
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                total_len += strlen(list->tuples[i].items[j]) + 1;
            }
        }
    }

    if (total_len == 0) {
        char *empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    char *result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t pos = 0;

    for (i = 0; i < list->count; i++) {
        for (j = 0; j < list->tuples[i].count; j++) {
            if (list->tuples[i].items[j] != NULL) {
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                size_t len = strlen(list->tuples[i].items[j]);
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + pos, list->tuples[i].items[j], len);
                pos += len;
                result[pos++] = ' ';
            }
        }
    }

    if (pos > 0) {
        result[pos - 1] = '\0';
    } else {
        result[pos] = '\0';
    }

    return result;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }

    size_t i, j;
    for (i = 0; i < list->count; i++) {
        for (j = 0; j < list->tuples[i].count; j++) {
            free(list->tuples[i].items[j]);
        }
        free(list->tuples[i].items);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

int main(void) {
    TupleList list;
    list.count = 2;
    list.tuples = malloc(list.count * sizeof(Tuple));
    if (list.tuples == NULL) {
        return EXIT_FAILURE;
    }

    list.tuples[0].count = 2;
    list.tuples[0].items = malloc(list.tuples[0].count * sizeof(char *));
    if (list.tuples[0].items == NULL) {
        free(list.tuples);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    list.tuples[0].items[0] = malloc(strlen("Hello") + 1);
    if (list.tuples[0].items[0] == NULL) {
        free(list.tuples[0].items);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list.tuples[0].items[0], "Hello");

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    list.tuples[0].items[1] = malloc(strlen("World") + 1);
    if (list.tuples[0].items[1] == NULL) {
        free(list.tuples[0].items[0]);
        free(list.tuples[0].items);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list.tuples[0].items[1], "World");

    list.tuples[1].count = 3;
    list.tuples[1].items = malloc(list.tuples[1].count * sizeof(char *));
    if (list.tuples[1].items == NULL) {
        free(list.tuples[0].items[1]);
        free(list.tuples[0].items[0]);
        free(list.tuples[0].items);
        free(list.tuples);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    list.tuples[1].items[0] = malloc(strlen("C") + 1);
    if (list.tuples[1].items[0] == NULL) {
        free(list.tuples[1].items);
        free(list.tuples[0].items[1]);
        free(list.tuples[0].items[0]);
        free(list.tuples[0].items);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant character. (risk 1, buffer)
     */
    strcpy(list.tuples[1].items[0], "C");

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    list.tuples[1].items[1] = malloc(strlen("Programming") + 1);
    if (list.tuples[1].items[1] == NULL) {
        free(list.tuples[1].items[0]);
        free(list.tuples[1].items);
        free(list.tuples[0].items[1]);
        free(list.tuples[0].items[0]);
        free(list.tuples[0].items);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list.tuples[1].items[1], "Programming");

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    list.tuples[1].items[2] = malloc(strlen("Language") + 1);
    if (list.tuples[1].items[2] == NULL) {
        free(list.tuples[1].items[1]);
        free(list.tuples[1].items[0]);
        free(list.tuples[1].items);
        free(list.tuples[0].items[1]);
        free(list.tuples[0].items[0]);
        free(list.tuples[0].items);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list.tuples[1].items[2], "Language");

    char *flattened = flatten_tuple_list(&list);
    if (flattened != NULL) {
        printf("%s\n", flattened);
        free(flattened);
    }

    free_tuple_list(&list);

    return EXIT_SUCCESS;
}