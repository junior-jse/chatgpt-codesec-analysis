#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Heap {
    int *data;
    size_t size;
    size_t capacity;
} Heap;

static int heap_init(Heap *heap, size_t capacity) {
    if (!heap || capacity == 0) {
        return 0;
    }
    
    heap->data = (int *)malloc(sizeof(int) * capacity);
    if (!heap->data) {
        return 0;
    }
    
    heap->size = 0;
    heap->capacity = capacity;
    return 1;
}

static void heap_destroy(Heap *heap) {
    if (!heap) {
        return;
    }
    
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

static void heap_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void heap_sift_up(Heap *heap, size_t index) {
    if (!heap || index == 0) {
        return;
    }
    
    size_t parent = (index - 1) / 2;
    
    if (heap->data[index] < heap->data[parent]) {
        heap_swap(&heap->data[index], &heap->data[parent]);
        heap_sift_up(heap, parent);
    }
}

static void heap_sift_down(Heap *heap, size_t index) {
    if (!heap) {
        return;
    }
    
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;
    
    if (left < heap->size && heap->data[left] < heap->data[smallest]) {
        smallest = left;
    }
    
    if (right < heap->size && heap->data[right] < heap->data[smallest]) {
        smallest = right;
    }
    
    if (smallest != index) {
        heap_swap(&heap->data[index], &heap->data[smallest]);
        heap_sift_down(heap, smallest);
    }
}

static int heap_push(Heap *heap, int value) {
    if (!heap || !heap->data) {
        return 0;
    }
    
    if (heap->size >= heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        int *new_data = (int *)realloc(heap->data, sizeof(int) * new_capacity);
        if (!new_data) {
            return 0;
        }
        heap->data = new_data;
        heap->capacity = new_capacity;
    }
    
    heap->data[heap->size] = value;
    heap_sift_up(heap, heap->size);
    heap->size++;
    
    return 1;
}

static int heap_pop(Heap *heap, int *out_value) {
    if (!heap || !heap->data || heap->size == 0 || !out_value) {
        return 0;
    }
    
    *out_value = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) {
        heap_sift_down(heap, 0);
    }
    
    return 1;
}

static int heap_peek(const Heap *heap, int *out_value) {
    if (!heap || !heap->data || heap->size == 0 || !out_value) {
        return 0;
    }
    
    *out_value = heap->data[0];
    return 1;
}

static size_t heap_size(const Heap *heap) {
    if (!heap) {
        return 0;
    }
    
    return heap->size;
}

int main(void) {
    int values[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    size_t num_values = sizeof(values) / sizeof(values[0]);
    Heap heap;
    size_t i;
    int value;
    
    if (!heap_init(&heap, num_values)) {
        fprintf(stderr, "Failed to initialize heap\n");
        return EXIT_FAILURE;
    }
    
    for (i = 0; i < num_values; i++) {
        if (!heap_push(&heap, values[i])) {
            fprintf(stderr, "Failed to push value\n");
            heap_destroy(&heap);
            return EXIT_FAILURE;
        }
    }
    
    printf("Sorted values: ");
    while (heap_size(&heap) > 0) {
        if (!heap_pop(&heap, &value)) {
            fprintf(stderr, "Failed to pop value\n");
            heap_destroy(&heap);
            return EXIT_FAILURE;
        }
        printf("%d ", value);
    }
    printf("\n");
    
    heap_destroy(&heap);
    
    return EXIT_SUCCESS;
}