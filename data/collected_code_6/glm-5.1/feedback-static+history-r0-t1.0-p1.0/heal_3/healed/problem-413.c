#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
    int c;
} Tuple;

int extract_nth_element(const Tuple *list, size_t list_size, size_t tuple_index, size_t element_n, int *out_value) {
    if (list == NULL || out_value == NULL) {
        return -1;
    }
    if (tuple_index >= list_size) {
        return -1;
    }
    if (element_n >= 3) {
        return -1;
    }

    const Tuple *t = &list[tuple_index];

    switch (element_n) {
        case 0:
            *out_value = t->a;
            break;
        case 1:
            *out_value = t->b;
            break;
        case 2:
            *out_value = t->c;
            break;
        default:
            return -1;
    }

    return 0;
}

int main() {
    Tuple list[] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    size_t list_size = sizeof(list) / sizeof(list[0]);
    int value = 0;

    if (extract_nth_element(list, list_size, 1, 2, &value) == 0) {
        printf("%d\n", value);
    } else {
        printf("Error\n");
    }

    if (extract_nth_element(list, list_size, 3, 0, &value) == 0) {
        printf("%d\n", value);
    } else {
        printf("Error\n");
    }

    if (extract_nth_element(NULL, list_size, 0, 0, &value) == 0) {
        printf("%d\n", value);
    } else {
        printf("Error\n");
    }

    return 0;
}