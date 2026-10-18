#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MinHeap;

static void heap_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify_up(MinHeap *heap, size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap->data[parent] <= heap->data[index]) {
            break;
        }
        heap_swap(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static void heapify_down(MinHeap *heap, size_t index) {
    while (1) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t smallest = index;
        
        if (left < heap->size && heap->data[left] < heap->data[smallest]) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right] < heap->data[smallest]) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        heap_swap(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

int heap_init(MinHeap *heap, size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 16;
    }
    heap->data = (int *)malloc(initial_capacity * sizeof(int));
    if (heap->data == NULL) {
        return 0;
    }
    heap->size = 0;
    heap->capacity = initial_capacity;
    return 1;
}

void heap_destroy(MinHeap *heap) {
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

int heap_push(MinHeap *heap, int value) {
    if (heap->size == heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        int *new_data = (int *)realloc(heap->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return 0;
        }
        heap->data = new_data;
        heap->capacity = new_capacity;
    }
    heap->data[heap->size] = value;
    heapify_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_pop(MinHeap *heap, int *out_value) {
    if (heap->size == 0) {
        return 0;
    }
    *out_value = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0);
    }
    return 1;
}

int heap_is_empty(const MinHeap *heap) {
    return heap->size == 0;
}

void push_all_and_pop_sorted(const int *values, size_t count, int *sorted_output) {
    MinHeap heap;
    size_t i;
    
    if (!heap_init(&heap, count > 0 ? count : 16)) {
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < count; i++) {
        if (!heap_push(&heap, values[i])) {
            heap_destroy(&heap);
            exit(EXIT_FAILURE);
        }
    }
    
    for (i = 0; i < count; i++) {
        if (!heap_pop(&heap, &sorted_output[i])) {
            heap_destroy(&heap);
            exit(EXIT_FAILURE);
        }
    }
    
    heap_destroy(&heap);
}

int main(void) {
    int values[] = {42, 17, 3, 88, 25, 9, 71, 56, 34, 12};
    size_t count = sizeof(values) / sizeof(values[0]);
    int *sorted = (int *)malloc(count * sizeof(int));
    size_t i;
    
    if (sorted == NULL) {
        return EXIT_FAILURE;
    }
    
    push_all_and_pop_sorted(values, count, sorted);
    
    for (i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");
    
    free(sorted);
    return EXIT_SUCCESS;
}