#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntList;

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntHeap;

static int int_list_init(IntList *list, size_t capacity) {
    if (list == NULL) {
        return -1;
    }
    if (capacity == 0) {
        list->data = NULL;
        list->size = 0;
        list->capacity = 0;
        return 0;
    }
    list->data = (int *)malloc(capacity * sizeof(int));
    if (list->data == NULL) {
        list->size = 0;
        list->capacity = 0;
        return -1;
    }
    list->size = 0;
    list->capacity = capacity;
    return 0;
}

static void int_list_destroy(IntList *list) {
    if (list == NULL) {
        return;
    }
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

static int int_list_append(IntList *list, int value) {
    if (list == NULL) {
        return -1;
    }
    if (list->size >= list->capacity) {
        size_t new_capacity = (list->capacity == 0) ? 8 : list->capacity * 2;
        int *new_data = (int *)realloc(list->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size] = value;
    list->size++;
    return 0;
}

static void int_heap_destroy(IntHeap *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

static void swap_ints(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void sift_down(int *data, size_t size, size_t index) {
    size_t largest = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (left < size && data[left] > data[largest]) {
        largest = left;
    }
    if (right < size && data[right] > data[largest]) {
        largest = right;
    }
    if (largest != index) {
        swap_ints(&data[index], &data[largest]);
        sift_down(data, size, largest);
    }
}

static int list_to_heap(const IntList *list, IntHeap *heap) {
    if (list == NULL || heap == NULL) {
        return -1;
    }
    if (list->size == 0) {
        heap->data = NULL;
        heap->size = 0;
        heap->capacity = 0;
        return 0;
    }
    heap->data = (int *)malloc(list->size * sizeof(int));
    if (heap->data == NULL) {
        heap->size = 0;
        heap->capacity = 0;
        return -1;
    }
    memcpy(heap->data, list->data, list->size * sizeof(int));
    heap->size = list->size;
    heap->capacity = list->size;

    for (size_t i = heap->size / 2; i > 0; i--) {
        sift_down(heap->data, heap->size, i - 1);
    }
    return 0;
}

static void print_array(const int *data, size_t size) {
    if (data == NULL) {
        printf("[]\n");
        return;
    }
    printf("[");
    for (size_t i = 0; i < size; i++) {
        printf("%d", data[i]);
        if (i + 1 < size) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    IntList list;
    IntHeap heap;
    int result = EXIT_SUCCESS;

    if (int_list_init(&list, 0) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return EXIT_FAILURE;
    }

    int values[] = {9, 4, 7, 1, 8, 3, 6, 2, 5};
    size_t count = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < count; i++) {
        if (int_list_append(&list, values[i]) != 0) {
            fprintf(stderr, "Failed to append value\n");
            int_list_destroy(&list);
            return EXIT_FAILURE;
        }
    }

    if (list_to_heap(&list, &heap) != 0) {
        fprintf(stderr, "Failed to convert list to heap\n");
        int_list_destroy(&list);
        return EXIT_FAILURE;
    }

    printf("Original list: ");
    print_array(list.data, list.size);
    printf("Heap:          ");
    print_array(heap.data, heap.size);

    int_list_destroy(&list);
    int_heap_destroy(&heap);

    return result;
}