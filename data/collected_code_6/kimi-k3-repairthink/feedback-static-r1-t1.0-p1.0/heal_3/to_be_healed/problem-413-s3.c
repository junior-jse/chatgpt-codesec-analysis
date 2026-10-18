#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char *name;
    double value;
} Tuple;

static int tuple_init(Tuple *t, int id, const char *name, double value) {
    size_t len;

    if (t == NULL || name == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(name) + 1;
    t->name = malloc(len);
    if (t->name == NULL) {
        t->name = NULL;
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(t->name, name, len);
    t->id = id;
    t->value = value;
    return 0;
}

static void tuple_destroy(Tuple *t) {
    if (t != NULL) {
        free(t->name);
        t->name = NULL;
    }
}

int extract_nth_element(const Tuple *list, size_t size, size_t n, Tuple *result) {
    if (list == NULL || result == NULL) {
        return -1;
    }
    if (n >= size) {
        return -1;
    }
    return tuple_init(result, list[n].id, list[n].name, list[n].value);
}

int main(void) {
    Tuple list[5];
    const size_t size = sizeof(list) / sizeof(list[0]);
    const size_t n = 2;
    Tuple result;
    size_t i;
    int status = EXIT_FAILURE;

    for (i = 0; i < size; i++) {
        list[i].name = NULL;
    }
    result.name = NULL;

    if (tuple_init(&list[0], 1, "Alice", 100.5) != 0 ||
        tuple_init(&list[1], 2, "Bob", 200.75) != 0 ||
        tuple_init(&list[2], 3, "Charlie", 300.25) != 0 ||
        tuple_init(&list[3], 4, "Diana", 400.0) != 0 ||
        tuple_init(&list[4], 5, "Eve", 500.5) != 0) {
        fprintf(stderr, "Error: Failed to initialize list\n");
        goto cleanup;
    }

    if (extract_nth_element(list, size, n, &result) == 0) {
        printf("Element at index %zu: id=%d, name=%s, value=%.2f\n",
               n, result.id, result.name, result.value);
        status = EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Error: Invalid index or NULL pointer\n");
    }

cleanup:
    tuple_destroy(&result);
    for (i = 0; i < size; i++) {
        tuple_destroy(&list[i]);
    }
    return status;
}