#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *data;
    size_t size;
    size_t capacity;
} TupleList;

int tuple_list_init(TupleList *list, size_t capacity) {
    list->data = (Tuple *)malloc(capacity * sizeof(Tuple));
    if (!list->data) {
        return -1;
    }
    list->size = 0;
    list->capacity = capacity;
    return 0;
}

int tuple_list_append(TupleList *list, Tuple t) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 1 : list->capacity * 2;
        Tuple *new_data = (Tuple *)realloc(list->data, new_capacity * sizeof(Tuple));
        if (!new_data) {
            return -1;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size++] = t;
    return 0;
}

int tuple_list_extract_rear(TupleList *list, Tuple *out) {
    if (!list || list->size == 0 || !out) {
        return -1;
    }
    *out = list->data[list->size - 1];
    list->size--;
    return 0;
}

void tuple_list_free(TupleList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

int main(void) {
    TupleList list;
    if (tuple_list_init(&list, 2) != 0) {
        return EXIT_FAILURE;
    }

    Tuple t1 = {1, 2};
    Tuple t2 = {3, 4};
    Tuple t3 = {5, 6};

    if (tuple_list_append(&list, t1) != 0) {
        tuple_list_free(&list);
        return EXIT_FAILURE;
    }
    if (tuple_list_append(&list, t2) != 0) {
        tuple_list_free(&list);
        return EXIT_FAILURE;
    }
    if (tuple_list_append(&list, t3) != 0) {
        tuple_list_free(&list);
        return EXIT_FAILURE;
    }

    Tuple rear;
    if (tuple_list_extract_rear(&list, &rear) == 0) {
        printf("Extracted rear: (%d, %d)\n", rear.a, rear.b);
    } else {
        tuple_list_free(&list);
        return EXIT_FAILURE;
    }

    if (tuple_list_extract_rear(&list, &rear) == 0) {
        printf("Extracted rear: (%d, %d)\n", rear.a, rear.b);
    }

    if (tuple_list_extract_rear(&list, &rear) == 0) {
        printf("Extracted rear: (%d, %d)\n", rear.a, rear.b);
    }

    int result = tuple_list_extract_rear(&list, &rear);
    if (result != 0) {
        printf("List is empty, cannot extract rear.\n");
    }

    tuple_list_free(&list);
    return EXIT_SUCCESS;
}