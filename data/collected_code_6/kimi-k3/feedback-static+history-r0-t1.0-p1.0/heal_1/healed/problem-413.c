#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
} Tuple;

int get_nth_element(const Tuple *list, size_t size, size_t n, int *result) {
    if (list == NULL || result == NULL || n >= size) {
        return 0;
    }
    *result = list[n].a;
    return 1;
}

int main(void) {
    Tuple list[] = {{1}, {3}, {5}, {7}, {9}};
    size_t size = sizeof(list) / sizeof(list[0]);
    size_t n = 2;
    int result;

    if (get_nth_element(list, size, n, &result)) {
        printf("Element at index %zu: %d\n", n, result);
    } else {
        printf("Invalid index or error occurred.\n");
    }

    return 0;
}