#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    int value;
    size_t list_index;
    size_t element_index;
} MergeNode;

typedef struct {
    int value;
    size_t frequency;
} FrequencyNode;

static int merge_less(const MergeNode *a, const MergeNode *b)
{
    if (a->value != b->value) {
        return a->value < b->value;
    }
    return a->list_index < b->list_index;
}

static void merge_heap_push(MergeNode *heap, size_t *size, MergeNode node)
{
    size_t index = (*size)++;

    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!merge_less(&node, &heap[parent])) {
            break;
        }

        heap[index] = heap[parent];
        index = parent;
    }

    heap[index] = node;
}

static MergeNode merge_heap_pop(MergeNode *heap, size_t *size)
{
    MergeNode root = heap[0];
    MergeNode last = heap[--(*size)];
    size_t index = 0U;

    while (index < *size) {
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;
        size_t smallest;

        if (left >= *size) {
            break;
        }

        smallest = left;
        if (right < *size && merge_less(&heap[right], &heap[left])) {
            smallest = right;
        }

        if (!merge_less(&heap[smallest], &last)) {
            break;
        }

        heap[index] = heap[smallest];
        index = smallest;
    }

    if (*size > 0U) {
        heap[index] = last;
    }

    return root;
}

static int frequency_less(const FrequencyNode *a, const FrequencyNode *b)
{
    if (a->frequency != b->frequency) {
        return a->frequency < b->frequency;
    }
    return a->value > b->value;
}

static void frequency_heap_push(FrequencyNode *heap,
                                size_t *size,
                                FrequencyNode node)
{
    size_t index = (*size)++;

    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!frequency_less(&node, &heap[parent])) {
            break;
        }

        heap[index] = heap[parent];
        index = parent;
    }

    heap[index] = node;
}

static FrequencyNode frequency_heap_pop(FrequencyNode *heap, size_t *size)
{
    FrequencyNode root = heap[0];
    FrequencyNode last = heap[--(*size)];
    size_t index = 0U;

    while (index < *size) {
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;
        size_t smallest;

        if (left >= *size) {
            break;
        }

        smallest = left;
        if (right < *size &&
            frequency_less(&heap[right], &heap[left])) {
            smallest = right;
        }

        if (!frequency_less(&heap[smallest], &last)) {
            break;
        }

        heap[index] = heap[smallest];
        index = smallest;
    }

    if (*size > 0U) {
        heap[index] = last;
    }

    return root;
}

static void consider_frequency(FrequencyNode *heap,
                               size_t *heap_size,
                               size_t k,
                               int value,
                               size_t frequency)
{
    FrequencyNode node = { value, frequency };

    if (*heap_size < k) {
        frequency_heap_push(heap, heap_size, node);
    } else if (frequency_less(&heap[0], &node)) {
        (void)frequency_heap_pop(heap, heap_size);
        frequency_heap_push(heap, heap_size, node);
    }
}

static int output_order_compare(const void *left, const void *right)
{
    const FrequencyNode *a = left;
    const FrequencyNode *b = right;

    if (a->frequency < b->frequency) {
        return 1;
    }
    if (a->frequency > b->frequency) {
        return -1;
    }
    if (a->value < b->value) {
        return -1;
    }
    if (a->value > b->value) {
        return 1;
    }
    return 0;
}

static int top_k_frequent(const int *const lists[],
                          const size_t lengths[],
                          size_t list_count,
                          size_t k,
                          FrequencyNode **result,
                          size_t *result_count)
{
    MergeNode *merge_heap = NULL;
    FrequencyNode *top_heap = NULL;
    size_t merge_size = 0U;
    size_t top_size = 0U;
    int current_value = 0;
    size_t current_frequency = 0U;
    int have_current = 0;

    if (result == NULL || result_count == NULL ||
        (list_count > 0U && (lists == NULL || lengths == NULL))) {
        return -1;
    }

    *result = NULL;
    *result_count = 0U;

    if (k == 0U || list_count == 0U) {
        return 0;
    }

    if (list_count > SIZE_MAX / sizeof(*merge_heap) ||
        k > SIZE_MAX / sizeof(*top_heap)) {
        return -1;
    }

    merge_heap = malloc(list_count * sizeof(*merge_heap));
    top_heap = malloc(k * sizeof(*top_heap));

    if (merge_heap == NULL || top_heap == NULL) {
        free(merge_heap);
        free(top_heap);
        return -1;
    }

    for (size_t i = 0U; i < list_count; ++i) {
        if (lengths[i] > 0U) {
            MergeNode node;

            if (lists[i] == NULL) {
                free(merge_heap);
                free(top_heap);
                return -1;
            }

            node.value = lists[i][0];
            node.list_index = i;
            node.element_index = 0U;
            merge_heap_push(merge_heap, &merge_size, node);
        }
    }

    while (merge_size > 0U) {
        MergeNode node = merge_heap_pop(merge_heap, &merge_size);

        if (!have_current || node.value != current_value) {
            if (have_current) {
                consider_frequency(top_heap, &top_size, k,
                                   current_value, current_frequency);
            }
            current_value = node.value;
            current_frequency = 1U;
            have_current = 1;
        } else {
            ++current_frequency;
        }

        ++node.element_index;
        if (node.element_index < lengths[node.list_index]) {
            node.value = lists[node.list_index][node.element_index];
            merge_heap_push(merge_heap, &merge_size, node);
        }
    }

    if (have_current) {
        consider_frequency(top_heap, &top_size, k,
                           current_value, current_frequency);
    }

    qsort(top_heap, top_size, sizeof(*top_heap), output_order_compare);

    free(merge_heap);
    *result = top_heap;
    *result_count = top_size;
    return 0;
}

int main(void)
{
    static const int list0[] = { 1, 2, 4, 7, 10 };
    static const int list1[] = { 1, 3, 4, 7, 9 };
    static const int list2[] = { 2, 4, 5, 7, 10 };
    static const int list3[] = { 1, 4, 6, 7, 8 };
    static const int *const lists[] = { list0, list1, list2, list3 };
    static const size_t lengths[] = {
        sizeof(list0) / sizeof(list0[0]),
        sizeof(list1) / sizeof(list1[0]),
        sizeof(list2) / sizeof(list2[0]),
        sizeof(list3) / sizeof(list3[0])
    };
    const size_t k = 3U;
    FrequencyNode *result = NULL;
    size_t result_count = 0U;

    if (top_k_frequent(lists,
                       lengths,
                       sizeof(lists) / sizeof(lists[0]),
                       k,
                       &result,
                       &result_count) != 0) {
        fputs("Failed to find the top-k integers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < result_count; ++i) {
        printf("%d %zu\n", result[i].value, result[i].frequency);
    }

    free(result);
    return EXIT_SUCCESS;
}