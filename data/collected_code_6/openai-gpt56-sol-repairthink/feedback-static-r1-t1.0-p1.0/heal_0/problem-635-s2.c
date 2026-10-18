#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MinHeap;

static int heap_init(MinHeap *heap, size_t capacity)
{
    if (heap == NULL) {
        return 0;
    }

    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;

    if (capacity == 0) {
        return 1;
    }

    if (capacity > SIZE_MAX / sizeof(*heap->data)) {
        return 0;
    }

    heap->data = malloc(capacity * sizeof(*heap->data));
    if (heap->data == NULL) {
        return 0;
    }

    heap->capacity = capacity;
    return 1;
}

static void heap_destroy(MinHeap *heap)
{
    if (heap != NULL) {
        free(heap->data);
        heap->data = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static int heap_push(MinHeap *heap, int value)
{
    size_t index;

    if (heap == NULL || heap->size >= heap->capacity) {
        return 0;
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
    return 1;
}

static int heap_pop(MinHeap *heap, int *value)
{
    int last;
    size_t index;

    if (heap == NULL || value == NULL || heap->size == 0) {
        return 0;
    }

    *value = heap->data[0];
    --heap->size;

    if (heap->size == 0) {
        return 1;
    }

    last = heap->data[heap->size];
    index = 0;

    for (;;) {
        size_t left = index * 2 + 1;
        size_t right;
        size_t smallest;

        if (left >= heap->size) {
            break;
        }

        right = left + 1;
        smallest = left;

        if (right < heap->size &&
            heap->data[right] < heap->data[left]) {
            smallest = right;
        }

        if (heap->data[smallest] >= last) {
            break;
        }

        heap->data[index] = heap->data[smallest];
        index = smallest;
    }

    heap->data[index] = last;
    return 1;
}

static int push_and_pop_smallest(const int *values, size_t count)
{
    MinHeap heap;
    size_t i;
    int value;

    if (values == NULL && count != 0) {
        return 0;
    }

    if (!heap_init(&heap, count)) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (!heap_push(&heap, values[i])) {
            heap_destroy(&heap);
            return 0;
        }
    }

    while (heap_pop(&heap, &value)) {
        if (printf("%d\n", value) < 0) {
            heap_destroy(&heap);
            return 0;
        }
    }

    heap_destroy(&heap);
    return 1;
}

int main(void)
{
    const int values[] = {42, 7, 19, -3, 7, 100, 0};
    const size_t count = sizeof(values) / sizeof(values[0]);

    if (!push_and_pop_smallest(values, count)) {
        fputs("Heap operation failed\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}