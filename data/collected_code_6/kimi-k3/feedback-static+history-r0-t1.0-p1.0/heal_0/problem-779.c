#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} List;

typedef struct {
    List *lists;
    size_t count;
    size_t capacity;
} ListContainer;

int compare_lists(const List *a, const List *b) {
    if (a == NULL || b == NULL) {
        return (a == b) ? 0 : (a == NULL) ? -1 : 1;
    }
    if (a->size != b->size) {
        return (a->size < b->size) ? -1 : 1;
    }
    if (a->size == 0) {
        return 0;
    }
    if (a->data == NULL || b->data == NULL) {
        return (a->data == b->data) ? 0 : (a->data == NULL) ? -1 : 1;
    }
    return memcmp(a->data, b->data, a->size * sizeof(int));
}

int compare_lists_qsort(const void *a, const void *b) {
    return compare_lists((const List *)a, (const List *)b);
}

size_t count_unique_lists(ListContainer *container) {
    if (container == NULL || container->lists == NULL || container->count == 0) {
        return 0;
    }
    
    qsort(container->lists, container->count, sizeof(List), compare_lists_qsort);
    
    size_t unique_count = 1;
    for (size_t i = 1; i < container->count; i++) {
        if (compare_lists(&container->lists[i-1], &container->lists[i]) != 0) {
            unique_count++;
        }
    }
    
    return unique_count;
}

int init_container(ListContainer *container, size_t initial_capacity) {
    if (container == NULL || initial_capacity == 0) {
        return 0;
    }
    
    container->lists = malloc(initial_capacity * sizeof(List));
    if (container->lists == NULL) {
        return 0;
    }
    
    container->count = 0;
    container->capacity = initial_capacity;
    return 1;
}

int add_list(ListContainer *container, const int *data, size_t size) {
    if (container == NULL || (size > 0 && data == NULL)) {
        return 0;
    }
    
    if (container->count >= container->capacity) {
        size_t new_capacity = container->capacity * 2;
        List *new_lists = realloc(container->lists, new_capacity * sizeof(List));
        if (new_lists == NULL) {
            return 0;
        }
        container->lists = new_lists;
        container->capacity = new_capacity;
    }
    
    container->lists[container->count].data = NULL;
    container->lists[container->count].size = size;
    
    if (size > 0) {
        container->lists[container->count].data = malloc(size * sizeof(int));
        if (container->lists[container->count].data == NULL) {
            return 0;
        }
        memcpy(container->lists[container->count].data, data, size * sizeof(int));
    }
    
    container->count++;
    return 1;
}

void free_container(ListContainer *container) {
    if (container == NULL) {
        return;
    }
    
    if (container->lists != NULL) {
        for (size_t i = 0; i < container->count; i++) {
            free(container->lists[i].data);
        }
        free(container->lists);
    }
    
    container->lists = NULL;
    container->count = 0;
    container->capacity = 0;
}

int main(void) {
    ListContainer container;
    
    if (!init_container(&container, 4)) {
        fprintf(stderr, "Failed to initialize container\n");
        return EXIT_FAILURE;
    }
    
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    int data3[] = {1, 2, 3};
    int data4[] = {6};
    int data5[] = {4, 5};
    int data6[] = {1, 2, 3, 4};
    
    if (!add_list(&container, data1, 3) ||
        !add_list(&container, data2, 2) ||
        !add_list(&container, data3, 3) ||
        !add_list(&container, data4, 1) ||
        !add_list(&container, data5, 2) ||
        !add_list(&container, data6, 4) ||
        !add_list(&container, NULL, 0)) {
        fprintf(stderr, "Failed to add lists\n");
        free_container(&container);
        return EXIT_FAILURE;
    }
    
    size_t unique = count_unique_lists(&container);
    printf("Number of unique lists: %zu\n", unique);
    
    free_container(&container);
    return EXIT_SUCCESS;
}