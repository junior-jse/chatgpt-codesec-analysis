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

TupleList* tuple_list_create(size_t capacity) {
    TupleList *list = (TupleList*)malloc(sizeof(TupleList));
    if (!list) return NULL;
    list->data = (Tuple*)malloc(capacity * sizeof(Tuple));
    if (!list->data) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;
}

int tuple_list_append(TupleList *list, Tuple t) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        Tuple *new_data = (Tuple*)realloc(list->data, new_capacity * sizeof(Tuple));
        if (!new_data) return -1;
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size++] = t;
    return 0;
}

int tuple_list_extract_rear(TupleList *list, Tuple *out) {
    if (!list || list->size == 0) return -1;
    *out = list->data[list->size - 1];
    list->size--;
    return 0;
}

void tuple_list_destroy(TupleList *list) {
    if (list) {
        free(list->data);
        free(list);
    }
}

int main() {
    TupleList *list = tuple_list_create(2);
    if (!list) return 1;

    tuple_list_append(list, (Tuple){1, 10});
    tuple_list_append(list, (Tuple){2, 20});
    tuple_list_append(list, (Tuple){3, 30});

    Tuple rear;
    if (tuple_list_extract_rear(list, &rear) == 0) {
        printf("Extracted: (%d, %d)\n", rear.a, rear.b);
    } else {
        printf("Extraction failed.\n");
    }

    if (tuple_list_extract_rear(list, &rear) == 0) {
        printf("Extracted: (%d, %d)\n", rear.a, rear.b);
    } else {
        printf("Extraction failed.\n");
    }

    tuple_list_destroy(list);
    return 0;
}