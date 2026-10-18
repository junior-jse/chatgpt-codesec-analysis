#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *string;
    int *list;
    size_t list_length;
} Tuple;

static void tuple_destroy(Tuple *tuple)
{
    if (tuple == NULL) {
        return;
    }

    free(tuple->string);
    free(tuple->list);
    free(tuple);
}

static Tuple *tuple_create(const char *string, const int *list, size_t list_length)
{
    Tuple *tuple;
    size_t string_length;

    if (string == NULL || (list == NULL && list_length != 0)) {
        return NULL;
    }

    string_length = strlen(string);
    if (string_length == SIZE_MAX) {
        return NULL;
    }

    tuple = calloc(1, sizeof(*tuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->string = malloc(string_length + 1);
    if (tuple->string == NULL) {
        tuple_destroy(tuple);
        return NULL;
    }

    memcpy(tuple->string, string, string_length + 1);

    if (list_length != 0) {
        if (list_length > SIZE_MAX / sizeof(*tuple->list)) {
            tuple_destroy(tuple);
            return NULL;
        }

        tuple->list = malloc(list_length * sizeof(*tuple->list));
        if (tuple->list == NULL) {
            tuple_destroy(tuple);
            return NULL;
        }

        memcpy(tuple->list, list, list_length * sizeof(*tuple->list));
    }

    tuple->list_length = list_length;
    return tuple;
}

int main(void)
{
    const int list[] = {1, 2, 3, 4, 5};
    const size_t list_length = sizeof(list) / sizeof(list[0]);
    Tuple *tuple = tuple_create("example", list, list_length);

    if (tuple == NULL) {
        fputs("Failed to create tuple\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("(%s, [", tuple->string) < 0) {
        tuple_destroy(tuple);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tuple->list_length; ++i) {
        if (printf("%s%d", i == 0 ? "" : ", ", tuple->list[i]) < 0) {
            tuple_destroy(tuple);
            return EXIT_FAILURE;
        }
    }

    if (puts("])") == EOF) {
        tuple_destroy(tuple);
        return EXIT_FAILURE;
    }

    tuple_destroy(tuple);
    return EXIT_SUCCESS;
}