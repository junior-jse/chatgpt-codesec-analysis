#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MinHeap;

MinHeap* heap_create(size_t capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (int*)malloc(capacity * sizeof(int));
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_destroy(MinHeap *heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

void heap_push(MinHeap *heap, int value) {
    if (!heap || heap->size >= heap->capacity) return;
    size_t idx = heap->size++;
    heap->data[idx] = value;
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (heap->data[parent] > heap->data[idx]) {
            int temp = heap->data[parent];
            heap->data[parent] = heap->data[idx];
            heap->data[idx] = temp;
            idx = parent;
        } else {
            break;
        }
    }
}

int heap_pop(MinHeap *heap) {
    if (!heap || heap->size == 0) return 0;
    int min_val = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    size_t idx = 0;
    while (1) {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t smallest = idx;
        if (left < heap->size && heap->data[left] < heap->data[smallest]) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right] < heap->data[smallest]) {
            smallest = right;
        }
        if (smallest != idx) {
            int temp = heap->data[idx];
            heap->data[idx] = heap->data[smallest];
            heap->data[smallest] = temp;
            idx = smallest;
        } else {
            break;
        }
    }
    return min_val;
}

void heap_sort_and_print(int *values, size_t count) {
    MinHeap *heap = heap_create(count);
    if (!heap) return;
    for (size_t i = 0; i < count; ++i) {
        heap_push(heap, values[i]);
    }
    for (size_t i = 0; i < count; ++i) {
        printf("%d ", heap_pop(heap));
    }
    printf("\n");
    heap_destroy(heap);
}

int main(void) {
    int values[] = {42, 12, 18, 5, 33, 9, 7};
    size_t count = sizeof(values) / sizeof(values[0]);
    heap_sort_and_print(values, count);
    return 0;
}