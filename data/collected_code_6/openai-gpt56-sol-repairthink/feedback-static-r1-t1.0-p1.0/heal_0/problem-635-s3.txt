#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *values;
    size_t size;
    size_t capacity;
} MinHeap;

static int heap_init(MinHeap *heap, size_t capacity)
{
    if (heap == NULL || capacity == 0 ||
        capacity > SIZE_MAX / sizeof(*heap->values)) {
        return 0;
    }

    heap->values = malloc(capacity * sizeof(*heap->values));
    if (heap->values == NULL) {
        heap->size = 0;
        heap->capacity = 0;
        return 0;
    }

    heap->size = 0;
    heap->capacity = capacity;
    return 1;
}

static void heap_destroy(MinHeap *heap)
{
    if (heap != NULL) {
        free(heap->values);
        heap->values = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static int heap_push(MinHeap *heap, int value)
{
    size_t index;

    if (heap == NULL || heap->values == NULL || heap->size >= heap->capacity) {
        return 0;
    }

    index = heap->size++;
    while (index > 0) {
        size_t parent = (index - 1) / 2;

        if (heap->values[parent] <= value) {
            break;
        }

        heap->values[index] = heap->values[parent];
        index = parent;
    }

    heap->values[index] = value;
    return 1;
}

static int heap_pop(MinHeap *heap, int *value)
{
    size_t index;
    int replacement;

    if (heap == NULL || value == NULL || heap->values == NULL ||
        heap->size == 0) {
        return 0;
    }

    *value = heap->values[0];
    heap->size--;

    if (heap->size == 0) {
        return 1;
    }

    replacement = heap->values[heap->size];
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
            heap->values[right] < heap->values[left]) {
            smallest = right;
        }

        if (heap->values[smallest] >= replacement) {
            break;
        }

        heap->values[index] = heap->values[smallest];
        index = smallest;
    }

    heap->values[index] = replacement;
    return 1;
}

static int push_and_pop_values(const int *values, size_t count)
{
    MinHeap heap = {0};
    size_t i;
    int value;
    int success = 0;

    if (values == NULL || count == 0) {
        return 0;
    }

    if (!heap_init(&heap, count)) {
        fprintf(stderr, "Failed to initialize heap\n");
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (!heap_push(&heap, values[i])) {
            fprintf(stderr, "Failed to push value into heap\n");
            goto cleanup;
        }
    }

    while (heap.size > 0) {
        if (!heap_pop(&heap, &value)) {
            fprintf(stderr, "Failed to pop value from heap\n");
            goto cleanup;
        }

        if (printf("%d\n", value) < 0) {
            fprintf(stderr, "Failed to write output\n");
            goto cleanup;
        }
    }

    success = 1;

cleanup:
    heap_destroy(&heap);
    return success;
}

int main(void)
{
    const int values[] = {42, 7, 19, -3, 25, 7, 0, 81};
    const size_t count = sizeof(values) / sizeof(values[0]);

    if (!push_and_pop_values(values, count)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}