#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint64_t value;
    size_t prime_index;
    size_t ugly_index;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    size_t size;
    size_t capacity;
} MinHeap;

static int node_less(const HeapNode *a, const HeapNode *b)
{
    if (a->value != b->value) {
        return a->value < b->value;
    }
    if (a->prime_index != b->prime_index) {
        return a->prime_index < b->prime_index;
    }
    return a->ugly_index < b->ugly_index;
}

static void swap_nodes(HeapNode *a, HeapNode *b)
{
    HeapNode temporary = *a;
    *a = *b;
    *b = temporary;
}

static int heap_init(MinHeap *heap, size_t capacity)
{
    if (heap == NULL || capacity == 0 ||
        capacity > SIZE_MAX / sizeof(*heap->nodes)) {
        return 0;
    }

    heap->nodes = malloc(capacity * sizeof(*heap->nodes));
    if (heap->nodes == NULL) {
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
        free(heap->nodes);
        heap->nodes = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static int heap_push(MinHeap *heap, HeapNode node)
{
    size_t index;

    if (heap == NULL || heap->nodes == NULL || heap->size >= heap->capacity) {
        return 0;
    }

    index = heap->size++;
    heap->nodes[index] = node;

    while (index > 0) {
        size_t parent = (index - 1) / 2;

        if (!node_less(&heap->nodes[index], &heap->nodes[parent])) {
            break;
        }

        swap_nodes(&heap->nodes[index], &heap->nodes[parent]);
        index = parent;
    }

    return 1;
}

static int heap_pop(MinHeap *heap, HeapNode *result)
{
    size_t index = 0;

    if (heap == NULL || result == NULL || heap->nodes == NULL ||
        heap->size == 0) {
        return 0;
    }

    *result = heap->nodes[0];
    --heap->size;

    if (heap->size == 0) {
        return 1;
    }

    heap->nodes[0] = heap->nodes[heap->size];

    for (;;) {
        size_t left;
        size_t right;
        size_t smallest;

        if (index > (SIZE_MAX - 1) / 2) {
            break;
        }

        left = index * 2 + 1;
        if (left >= heap->size) {
            break;
        }

        right = left + 1;
        smallest = left;

        if (right < heap->size &&
            node_less(&heap->nodes[right], &heap->nodes[left])) {
            smallest = right;
        }

        if (!node_less(&heap->nodes[smallest], &heap->nodes[index])) {
            break;
        }

        swap_nodes(&heap->nodes[index], &heap->nodes[smallest]);
        index = smallest;
    }

    return 1;
}

static int find_nth_super_ugly_number(size_t n, const uint64_t primes[],
                                      size_t k, uint64_t *result)
{
    uint64_t *ugly = NULL;
    MinHeap heap = {0};
    uint64_t last = 1;
    size_t count = 1;
    size_t i;
    int success = 0;

    if (result == NULL || primes == NULL || n == 0 || k == 0 ||
        n > SIZE_MAX / sizeof(*ugly)) {
        return 0;
    }

    for (i = 0; i < k; ++i) {
        size_t j;

        if (primes[i] < 2) {
            return 0;
        }

        for (j = 0; j < i; ++j) {
            if (primes[i] == primes[j]) {
                return 0;
            }
        }
    }

    ugly = malloc(n * sizeof(*ugly));
    if (ugly == NULL) {
        return 0;
    }

    if (!heap_init(&heap, k)) {
        free(ugly);
        return 0;
    }

    ugly[0] = 1;

    for (i = 0; i < k; ++i) {
        HeapNode node = {primes[i], i, 0};

        if (!heap_push(&heap, node)) {
            goto cleanup;
        }
    }

    while (count < n) {
        HeapNode node;

        if (!heap_pop(&heap, &node)) {
            goto cleanup;
        }

        if (node.value > last) {
            ugly[count++] = node.value;
            last = node.value;
        }

        if (node.ugly_index + 1 < count) {
            uint64_t prime = primes[node.prime_index];
            uint64_t factor = ugly[node.ugly_index + 1];

            if (factor > UINT64_MAX / prime) {
                goto cleanup;
            }

            node.ugly_index++;
            node.value = factor * prime;

            if (!heap_push(&heap, node)) {
                goto cleanup;
            }
        } else {
            if (!heap_push(&heap, node)) {
                goto cleanup;
            }
        }
    }

    *result = ugly[n - 1];
    success = 1;

cleanup:
    heap_destroy(&heap);
    free(ugly);
    return success;
}

int main(void)
{
    uint64_t primes[] = {2, 7, 13, 19};
    uint64_t result;
    size_t n = 12;
    size_t k = sizeof(primes) / sizeof(primes[0]);

    if (!find_nth_super_ugly_number(n, primes, k, &result)) {
        fputs("Failed to compute the super ugly number.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("%llu\n", (unsigned long long)result);
    return EXIT_SUCCESS;
}