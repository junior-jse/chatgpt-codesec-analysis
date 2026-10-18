#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *data;
    size_t size;
    size_t capacity;
} List;

int list_init(List *list, size_t capacity) {
    list->data = (Tuple *)malloc(capacity * sizeof(Tuple));
    if (list->data == NULL) {
        return -1;
    }
    list->size = 0;
    list->capacity = capacity;
    return 0;
}

int list_add(List *list, Tuple tuple) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        Tuple *new_data = (Tuple *)realloc(list->data, new_capacity * sizeof(Tuple));
        if (new_data == NULL) {
            return -1;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size] = tuple;
    list->size++;
    return 0;
}

void list_free(List *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

int main(void) {
    List my_list;
    if (list_init(&my_list, 2) != 0) {
        return EXIT_FAILURE;
    }

    Tuple t1 = {1, 2};
    Tuple t2 = {3, 4};
    Tuple t3 = {5, 6};
    Tuple t4 = {7, 8};

    if (list_add(&my_list, t1) != 0) {
        list_free(&my_list);
        return EXIT_FAILURE;
    }

    if (list_add(&my_list, t2) != 0) {
        list_free(&my_list);
        return EXIT_FAILURE;
    }

    if (list_add(&my_list, t3) != 0) {
        list_free(&my_list);
        return EXIT_FAILURE;
    }

    if (list_add(&my_list, t4) != 0) {
        list_free(&my_list);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < my_list.size; ++i) {
        printf("(%d, %d)\n", my_list.data[i].a, my_list.data[i].b);
    }

    list_free(&my_list);
    
    if (my_list.data != NULL || my_list.size != 0 || my_list.capacity != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}