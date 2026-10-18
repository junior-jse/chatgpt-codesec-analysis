#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *entries;
    int size;
    int capacity;
} Dictionary;

typedef struct {
    Dictionary *items;
    int size;
    int capacity;
} List;

Dictionary create_dictionary() {
    Dictionary dict;
    dict.entries = NULL;
    dict.size = 0;
    dict.capacity = 0;
    return dict;
}

List create_list(int count) {
    List list;
    list.size = 0;
    list.capacity = 0;
    list.items = NULL;

    if (count < 0) {
        return list;
    }

    if (count == 0) {
        return list;
    }

    size_t alloc_size = 0;
    if (__builtin_mul_overflow(count, sizeof(Dictionary), &alloc_size)) {
        return list;
    }

    list.items = (Dictionary *)malloc(alloc_size);
    if (list.items == NULL) {
        return list;
    }

    list.size = count;
    list.capacity = count;

    for (int i = 0; i < count; i++) {
        list.items[i] = create_dictionary();
    }

    return list;
}

void free_dictionary(Dictionary *dict) {
    if (dict != NULL) {
        free(dict->entries);
        dict->entries = NULL;
        dict->size = 0;
        dict->capacity = 0;
    }
}

void free_list(List *list) {
    if (list != NULL) {
        if (list->items != NULL) {
            for (int i = 0; i < list->size; i++) {
                free_dictionary(&list->items[i]);
            }
            free(list->items);
            list->items = NULL;
        }
        list->size = 0;
        list->capacity = 0;
    }
}

int main() {
    List list = create_list(5);

    if (list.items != NULL) {
        printf("List created with %d dictionaries\n", list.size);
    } else {
        printf("Failed to create list\n");
    }

    free_list(&list);

    return 0;
}