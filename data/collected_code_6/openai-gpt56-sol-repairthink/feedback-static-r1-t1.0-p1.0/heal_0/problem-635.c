#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MinHeap;

static bool min_heap_init(MinHeap *heap, size_t capacity)
{
    if (heap == NULL) {
        return false;
    }

    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;

    if (capacity == 0) {
        return true;
    }

    if (capacity > SIZE_MAX / sizeof(*heap->data)) {
        return false;
    }

    heap->data = malloc(capacity * sizeof(*heap->data));
    if (heap->data == NULL) {
        return false;
    }

    heap->capacity = capacity;
    return true;
}

static void min_heap_destroy(MinHeap *heap)
{
    if (heap != NULL) {
        free(heap->data);
        heap->data = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static bool min_heap_grow(MinHeap *heap)
{
    size_t new_capacity;
    int *new_data;

    if (heap == NULL) {
        return false;
    }

    if (heap->capacity == 0) {
        new_capacity = 8;
    } else {
        if (heap->capacity > SIZE_MAX / 2) {
            return false;
        }
        new_capacity = heap->capacity * 2;
    }

    if (new_capacity > SIZE_MAX / sizeof(*heap->data)) {
        return false;
    }

    new_data = realloc(heap->data, new_capacity * sizeof(*heap->data));
    if (new_data == NULL) {
        return false;
    }

    heap->data = new_data;
    heap->capacity = new_capacity;
    return true;
}

static bool min_heap_push(MinHeap *heap, int value)
{
    size_t index;

    if (heap == NULL) {
        return false;
    }

    if (heap->size == heap->capacity && !min_heap_grow(heap)) {
        return false;
    }

    index = heap->size++;
    while (index > 0) {
        size_t parent = (index - 1) / 2;

        if (heap->data[parent] <= value) {
            break;
        }

        heap->data[index] = heap->data[parent];
        index = parent;
    }

    heap->data[index] = value;
    return true;
}

static bool min_heap_pop(MinHeap *heap, int *value)
{
    int last;
    size_t index;

    if (heap == NULL || value == NULL || heap->size == 0) {
        return false;
    }

    *value = heap->data[0];
    --heap->size;

    if (heap->size == 0) {
        return true;
    }

    last = heap->data[heap->size];
    index = 0;

    while (index <= (heap->size - 1) / 2) {
        size_t left = index * 2 + 1;
        size_t right = left + 1;
        size_t child = left;

        if (left >= heap->size) {
            break;
        }

        if (right < heap->size && heap->data[right] < heap->data[left]) {
            child = right;
        }

        if (heap->data[child] >= last) {
            break;
        }

        heap->data[index] = heap->data[child];
        index = child;
    }

    heap->data[index] = last;
    return true;
}

static bool print_values_in_ascending_order(const int *values, size_t count)
{
    MinHeap heap;
    size_t i;
    int value;

    if (values == NULL && count != 0) {
        return false;
    }

    if (!min_heap_init(&heap, count)) {
        return false;
    }

    for (i = 0; i < count; ++i) {
        if (!min_heap_push(&heap, values[i])) {
            min_heap_destroy(&heap);
            return false;
        }
    }

    while (min_heap_pop(&heap, &value)) {
        if (printf("%d\n", value) < 0) {
            min_heap_destroy(&heap);
            return false;
        }
    }

    min_heap_destroy(&heap);
    return true;
}

int main(void)
{
    const int values[] = {42, 7, 19, -3, 7, 25, 0, 11};
    const size_t count = sizeof(values) / sizeof(values[0]);

    if (!print_values_in_ascending_order(values, count)) {
        fputs("Operation failed\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}