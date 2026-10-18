#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int *elements;
    int size;
    struct Tuple *next;
} Tuple;

int extract_nth(Tuple *list, int tuple_index, int n, int *result) {
    if (list == NULL || tuple_index < 0 || n < 0 || result == NULL) {
        return 0;
    }

    Tuple *current = list;
    int i = 0;
    while (current != NULL && i < tuple_index) {
        current = current->next;
        i++;
    }

    if (current == NULL || n >= current->size) {
        return 0;
    }

    *result = current->elements[n];
    return 1;
}

/* Possible weaknesses found:
 *  Parameter 'elements' can be declared as pointer to const [constParameterPointer]
 */
Tuple *create_tuple(int *elements, int size) {
    if (elements == NULL || size <= 0) {
        return NULL;
    }

    Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->elements = (int *)malloc(sizeof(int) * size);
    if (tuple->elements == NULL) {
        free(tuple);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        tuple->elements[i] = elements[i];
    }
    tuple->size = size;
    tuple->next = NULL;
    return tuple;
}

void free_list(Tuple *list) {
    Tuple *current = list;
    while (current != NULL) {
        Tuple *next = current->next;
        free(current->elements);
        free(current);
        current = next;
    }
}

int main(void) {
    int elems1[] = {1, 2, 3};
    int elems2[] = {4, 5, 6, 7};
    int elems3[] = {8, 9};

    Tuple *list = create_tuple(elems1, 3);
    if (list == NULL) {
        return 1;
    }

    Tuple *t2 = create_tuple(elems2, 4);
    if (t2 == NULL) {
        free_list(list);
        return 1;
    }
    list->next = t2;

    Tuple *t3 = create_tuple(elems3, 2);
    if (t3 == NULL) {
        free_list(list);
        return 1;
    }
    t2->next = t3;

    int value;
    if (extract_nth(list, 0, 2, &value)) {
        printf("%d\n", value);
    } else {
        printf("extraction failed\n");
    }

    if (extract_nth(list, 1, 3, &value)) {
        printf("%d\n", value);
    } else {
        printf("extraction failed\n");
    }

    if (extract_nth(list, 2, 1, &value)) {
        printf("%d\n", value);
    } else {
        printf("extraction failed\n");
    }

    free_list(list);
    return 0;
}