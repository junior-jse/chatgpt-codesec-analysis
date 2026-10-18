#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Heap;

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int heap_init(Heap *heap, size_t capacity) {
    if (heap == NULL || capacity == 0) {
        return -1;
    }
    heap->data = malloc(capacity * sizeof(int));
    if (heap->data == NULL) {
        return -1;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return 0;
}

static void heap_free(Heap *heap) {
    if (heap != NULL) {
        free(heap->data);
        heap->data = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static int heap_push(Heap *heap, int value) {
    if (heap == NULL) {
        return -1;
    }
    if (heap->size >= heap->capacity) {
        return -1;
    }
    heap->data[heap->size] = value;
    size_t current = heap->size;
    heap->size++;
    
    while (current > 0) {
        size_t parent = (current - 1) / 2;
        if (heap->data[parent] <= heap->data[current]) {
            break;
        }
        swap(&heap->data[parent], &heap->data[current]);
        current = parent;
    }
    return 0;
}

static int heap_pop(Heap *heap, int *result) {
    if (heap == NULL || result == NULL) {
        return -1;
    }
    if (heap->size == 0) {
        return -1;
    }
    
    *result = heap->data[0];
    heap->size--;
    
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        size_t current = 0;
        
        while (1) {
            size_t left = 2 * current + 1;
            size_t right = 2 * current + 2;
            size_t smallest = current;
            
            if (left < heap->size && heap->data[left] < heap->data[smallest]) {
                smallest = left;
            }
            if (right < heap->size && heap->data[right] < heap->data[smallest]) {
                smallest = right;
            }
            if (smallest == current) {
                break;
            }
            swap(&heap->data[current], &heap->data[smallest]);
            current = smallest;
        }
    }
    return 0;
}

int main(void) {
    int values[] = {9, 4, 7, 1, 3, 8, 2, 6, 5};
    size_t count = sizeof(values) / sizeof(values[0]);
    Heap heap;
    int result;
    
    if (heap_init(&heap, count) != 0) {
        fprintf(stderr, "Failed to initialize heap\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (heap_push(&heap, values[i]) != 0) {
            fprintf(stderr, "Failed to push value %d\n", values[i]);
            heap_free(&heap);
            return EXIT_FAILURE;
        }
    }
    
    printf("Popped values in ascending order:\n");
    while (heap_pop(&heap, &result) == 0) {
        printf("%d ", result);
    }
    printf("\n");
    
    heap_free(&heap);
    return EXIT_SUCCESS;
}