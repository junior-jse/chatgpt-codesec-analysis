#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *items;
    size_t count;
    size_t capacity;
} TupleList;

void list_init(TupleList *list, size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 4;
    }
    list->items = (Tuple *)malloc(initial_capacity * sizeof(Tuple));
    if (list->items == NULL) {
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = initial_capacity;
}

void list_append(TupleList *list, Tuple t) {
    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        Tuple *new_items = (Tuple *)realloc(list->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            free(list->items);
            exit(EXIT_FAILURE);
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->count++] = t;
}

void list_free(TupleList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int tuple_match(Tuple t1, Tuple t2) {
    return (t1.a == t2.a && t1.b == t2.b) || (t1.a == t2.b && t1.b == t2.a);
}

TupleList find_intersection(TupleList *list1, TupleList *list2) {
    TupleList result;
    list_init(&result, list1->count < list2->count ? list1->count : list2->count);

    size_t list2_len = list2->count;
    int *visited = (int *)calloc(list2_len, sizeof(int));
    if (visited == NULL) {
        list_free(&result);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < list1->count; i++) {
        for (size_t j = 0; j < list2_len; j++) {
            if (!visited[j] && tuple_match(list1->items[i], list2->items[j])) {
                visited[j] = 1;
                list_append(&result, list1->items[i]);
                break;
            }
        }
    }

    free(visited);
    return result;
}

int main(void) {
    TupleList list1, list2;
    list_init(&list1, 4);
    list_init(&list2, 4);

    list_append(&list1, (Tuple){1, 2});
    list_append(&list1, (Tuple){3, 4});
    list_append(&list1, (Tuple){5, 6});
    list_append(&list1, (Tuple){7, 8});

    list_append(&list2, (Tuple){2, 1});
    list_append(&list2, (Tuple){9, 10});
    list_append(&list2, (Tuple){6, 5});
    list_append(&list2, (Tuple){11, 12});

    TupleList intersection = find_intersection(&list1, &list2);

    for (size_t i = 0; i < intersection.count; i++) {
        printf("(%d, %d)\n", intersection.items[i].a, intersection.items[i].b);
    }

    list_free(&list1);
    list_free(&list2);
    list_free(&intersection);

    return 0;
}