#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 64

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char name[NAME_SIZE];
    double value;
} Tuple;

static int tuple_init(Tuple *t, int id, const char *name, double value) {
    int written;

    if (t == NULL || name == NULL) {
        return -1;
    }
    written = snprintf(t->name, NAME_SIZE, "%s", name);
    if (written < 0 || (size_t)written >= NAME_SIZE) {
        t->name[0] = '\0';
        return -1;
    }
    t->id = id;
    t->value = value;
    return 0;
}

int extract_nth_element(const Tuple *list, size_t size, size_t n, Tuple *result) {
    if (list == NULL || result == NULL) {
        return -1;
    }
    if (n >= size) {
        return -1;
    }
    *result = list[n];
    return 0;
}

int main(void) {
    Tuple list[5];
    const size_t size = sizeof(list) / sizeof(list[0]);
    const size_t n = 2;
    Tuple result;

    if (tuple_init(&list[0], 1, "Alice", 100.5) != 0 ||
        tuple_init(&list[1], 2, "Bob", 200.75) != 0 ||
        tuple_init(&list[2], 3, "Charlie", 300.25) != 0 ||
        tuple_init(&list[3], 4, "Diana", 400.0) != 0 ||
        tuple_init(&list[4], 5, "Eve", 500.5) != 0) {
        fprintf(stderr, "Error: Failed to initialize list\n");
        return EXIT_FAILURE;
    }

    if (extract_nth_element(list, size, n, &result) == 0) {
        printf("Element at index %zu: id=%d, name=%.*s, value=%.2f\n",
               n, result.id, (int)(sizeof(result.name) - 1), result.name,
               result.value);
    } else {
        fprintf(stderr, "Error: Invalid index or NULL pointer\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}