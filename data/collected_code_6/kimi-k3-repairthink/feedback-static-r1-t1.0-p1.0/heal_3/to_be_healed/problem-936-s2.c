#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int id;
    char *name;
    float value;
} Tuple;

static char *duplicate_string(const char *s)
{
    size_t len;
    char *copy;

    if (s == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(s);
    copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, s, len + 1);
    return copy;
}

int tuple_init(Tuple *tuple, int id, const char *name, float value)
{
    char *name_copy;

    if (tuple == NULL || name == NULL) {
        return -1;
    }

    name_copy = duplicate_string(name);
    if (name_copy == NULL) {
        return -1;
    }

    tuple->id = id;
    tuple->name = name_copy;
    tuple->value = value;

    return 0;
}

void tuple_destroy(Tuple *tuple)
{
    if (tuple == NULL) {
        return;
    }

    free(tuple->name);
    tuple->name = NULL;
    tuple->id = 0;
    tuple->value = 0.0f;
}

int rearrange_tuples(Tuple *tuples, size_t tuple_count, const size_t *order, size_t order_count)
{
    Tuple *temp;
    size_t i;
    size_t j;

    if (tuples == NULL || order == NULL) {
        return -1;
    }

    if (tuple_count == 0 || order_count == 0) {
        return -1;
    }

    if (order_count > tuple_count) {
        return -1;
    }

    if (order_count > SIZE_MAX / sizeof(Tuple)) {
        return -1;
    }

    temp = (Tuple *)malloc(order_count * sizeof(Tuple));
    if (temp == NULL) {
        return -1;
    }

    for (i = 0; i < order_count; i++) {
        if (order[i] >= tuple_count) {
            for (j = 0; j < i; j++) {
                tuple_destroy(&temp[j]);
            }
            free(temp);
            temp = NULL;
            return -1;
        }

        temp[i].id = tuples[order[i]].id;
        temp[i].value = tuples[order[i]].value;
        temp[i].name = duplicate_string(tuples[order[i]].name);
        if (temp[i].name == NULL) {
            for (j = 0; j < i; j++) {
                tuple_destroy(&temp[j]);
            }
            free(temp);
            temp = NULL;
            return -1;
        }
    }

    for (i = 0; i < order_count; i++) {
        tuple_destroy(&tuples[i]);
        tuples[i] = temp[i];
    }

    free(temp);
    temp = NULL;

    return 0;
}

void print_tuples(const Tuple *tuples, size_t count)
{
    size_t i;

    if (tuples == NULL || count == 0) {
        return;
    }

    for (i = 0; i < count; i++) {
        printf("Tuple %zu: id=%d, name=%s, value=%.2f\n",
               i, tuples[i].id,
               tuples[i].name != NULL ? tuples[i].name : "(null)",
               tuples[i].value);
    }
}

int main(void)
{
    Tuple tuples[5];
    size_t order[] = {2, 0, 4, 1, 3};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);
    size_t i;
    int result;
    int status = EXIT_SUCCESS;

    for (i = 0; i < tuple_count; i++) {
        tuples[i].id = 0;
        tuples[i].name = NULL;
        tuples[i].value = 0.0f;
    }

    if (tuple_init(&tuples[0], 1, "Alpha", 10.5f) != 0 ||
        tuple_init(&tuples[1], 2, "Beta", 20.3f) != 0 ||
        tuple_init(&tuples[2], 3, "Gamma", 30.7f) != 0 ||
        tuple_init(&tuples[3], 4, "Delta", 40.1f) != 0 ||
        tuple_init(&tuples[4], 5, "Epsilon", 50.9f) != 0) {
        fprintf(stderr, "Error: Failed to initialize tuples\n");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    printf("Original tuples:\n");
    print_tuples(tuples, tuple_count);

    result = rearrange_tuples(tuples, tuple_count, order, order_count);

    if (result != 0) {
        fprintf(stderr, "Error: Failed to rearrange tuples\n");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    printf("\nRearranged tuples:\n");
    print_tuples(tuples, tuple_count);

cleanup:
    for (i = 0; i < tuple_count; i++) {
        tuple_destroy(&tuples[i]);
    }

    return status;
}