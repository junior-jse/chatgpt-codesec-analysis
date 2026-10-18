#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint64_t value;
    size_t prime_index;
    size_t ugly_index;
} HeapNode;

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

static void heap_push(HeapNode *heap, size_t *size, HeapNode node)
{
    size_t index = (*size)++;

    while (index > 0) {
        size_t parent = (index - 1U) / 2U;

        if (!node_less(&node, &heap[parent])) {
            break;
        }

        heap[index] = heap[parent];
        index = parent;
    }

    heap[index] = node;
}

static HeapNode heap_pop(HeapNode *heap, size_t *size)
{
    HeapNode root = heap[0];
    HeapNode last = heap[--(*size)];
    size_t index = 0;

    while (index < *size) {
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;
        size_t smallest;

        if (left >= *size) {
            break;
        }

        smallest = left;
        if (right < *size && node_less(&heap[right], &heap[left])) {
            smallest = right;
        }

        if (!node_less(&heap[smallest], &last)) {
            break;
        }

        heap[index] = heap[smallest];
        index = smallest;
    }

    if (*size > 0) {
        heap[index] = last;
    }

    return root;
}

static int compare_uint64(const void *lhs, const void *rhs)
{
    uint64_t a = *(const uint64_t *)lhs;
    uint64_t b = *(const uint64_t *)rhs;

    return (a > b) - (a < b);
}

static int nth_super_ugly_number(size_t n, const uint64_t *primes, size_t k,
                                uint64_t *result)
{
    uint64_t *unique_primes = NULL;
    uint64_t *ugly = NULL;
    HeapNode *heap = NULL;
    size_t unique_count = 0;
    size_t heap_size = 0;
    size_t i;
    int status = -1;

    if (n == 0 || primes == NULL || k == 0 || result == NULL) {
        return -1;
    }

    if (k > SIZE_MAX / sizeof(*unique_primes) ||
        n > SIZE_MAX / sizeof(*ugly) ||
        k > SIZE_MAX / sizeof(*heap)) {
        return -1;
    }

    unique_primes = malloc(k * sizeof(*unique_primes));
    ugly = malloc(n * sizeof(*ugly));
    heap = malloc(k * sizeof(*heap));

    if (unique_primes == NULL || ugly == NULL || heap == NULL) {
        goto cleanup;
    }

    for (i = 0; i < k; ++i) {
        if (primes[i] < 2) {
            goto cleanup;
        }
        unique_primes[i] = primes[i];
    }

    qsort(unique_primes, k, sizeof(*unique_primes), compare_uint64);

    for (i = 0; i < k; ++i) {
        if (unique_count == 0 ||
            unique_primes[i] != unique_primes[unique_count - 1U]) {
            unique_primes[unique_count++] = unique_primes[i];
        }
    }

    ugly[0] = 1;

    for (i = 0; i < unique_count; ++i) {
        HeapNode node = {
            .value = unique_primes[i],
            .prime_index = i,
            .ugly_index = 0
        };
        heap_push(heap, &heap_size, node);
    }

    for (i = 1; i < n; ++i) {
        HeapNode node;
        uint64_t next;

        if (heap_size == 0) {
            goto cleanup;
        }

        next = heap[0].value;
        ugly[i] = next;

        do {
            uint64_t prime;

            node = heap_pop(heap, &heap_size);
            ++node.ugly_index;

            if (node.ugly_index >= n) {
                goto cleanup;
            }

            prime = unique_primes[node.prime_index];
            if (ugly[node.ugly_index] > UINT64_MAX / prime) {
                goto cleanup;
            }

            node.value = ugly[node.ugly_index] * prime;
            heap_push(heap, &heap_size, node);
        } while (heap_size > 0 && heap[0].value == next);
    }

    *result = ugly[n - 1U];
    status = 0;

cleanup:
    free(heap);
    free(ugly);
    free(unique_primes);
    return status;
}

int main(void)
{
    uint64_t primes[] = {2, 7, 13, 19};
    const size_t k = sizeof(primes) / sizeof(primes[0]);
    const size_t n = 12;
    uint64_t result;

    if (nth_super_ugly_number(n, primes, k, &result) != 0) {
        fprintf(stderr, "Failed to compute the super ugly number.\n");
        return EXIT_FAILURE;
    }

    printf("%llu\n", (unsigned long long)result);
    return EXIT_SUCCESS;
}