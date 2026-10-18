#include <stdio.h>
#include <stdlib.h>
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

static int merge_node_less(const MergeNode *a, const MergeNode *b)
{
    if (a->value != b->value) {
        return a->value < b->value;
    }
    return a->list_index < b->list_index;
}

static void merge_heap_push(MergeNode *heap, size_t *size, MergeNode node)
{
    size_t index = (*size)++;

    heap[index] = node;
    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!merge_node_less(&heap[index], &heap[parent])) {
            break;
        }

        MergeNode temporary = heap[index];
        heap[index] = heap[parent];
        heap[parent] = temporary;
        index = parent;
    }
}

static MergeNode merge_heap_pop(MergeNode *heap, size_t *size)
{
    MergeNode result = heap[0];
    size_t new_size = --(*size);

    if (new_size > 0U) {
        size_t index = 0U;

        heap[0] = heap[new_size];
        for (;;) {
            size_t left = (index * 2U) + 1U;
            size_t right = left + 1U;
            size_t smallest = index;

            if (left < new_size &&
                merge_node_less(&heap[left], &heap[smallest])) {
                smallest = left;
            }

            if (right < new_size &&
                merge_node_less(&heap[right], &heap[smallest])) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            MergeNode temporary = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temporary;
            index = smallest;
        }
    }

    return result;
}

static int frequency_node_less(const FrequencyNode *a,
                               const FrequencyNode *b)
{
    if (a->frequency != b->frequency) {
        return a->frequency < b->frequency;
    }
    return a->value > b->value;
}

static void frequency_heap_push(FrequencyNode *heap, size_t *size,
                                FrequencyNode node)
{
    size_t index = (*size)++;

    heap[index] = node;
    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!frequency_node_less(&heap[index], &heap[parent])) {
            break;
        }

        FrequencyNode temporary = heap[index];
        heap[index] = heap[parent];
        heap[parent] = temporary;
        index = parent;
    }
}

static void frequency_heap_sift_down(FrequencyNode *heap, size_t size)
{
    size_t index = 0U;

    for (;;) {
        size_t left = (index * 2U) + 1U;
        size_t right = left + 1U;
        size_t smallest = index;

        if (left < size &&
            frequency_node_less(&heap[left], &heap[smallest])) {
            smallest = left;
        }

        if (right < size &&
            frequency_node_less(&heap[right], &heap[smallest])) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        FrequencyNode temporary = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temporary;
        index = smallest;
    }
}

static FrequencyNode frequency_heap_pop(FrequencyNode *heap, size_t *size)
{
    FrequencyNode result = heap[0];
    size_t new_size = --(*size);

    if (new_size > 0U) {
        heap[0] = heap[new_size];
        frequency_heap_sift_down(heap, new_size);
    }

    return result;
}

static int result_compare(const void *left, const void *right)
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

static int consider_frequency(FrequencyNode *heap, size_t *size, size_t k,
                              int value, size_t frequency)
{
    FrequencyNode node = { value, frequency };

    if (*size < k) {
        frequency_heap_push(heap, size, node);
    } else if (frequency_node_less(&heap[0], &node)) {
        heap[0] = node;
        frequency_heap_sift_down(heap, *size);
    }

    return 0;
}

static int find_top_k(const int *const lists[], const size_t lengths[],
                      size_t list_count, size_t k,
                      FrequencyNode **result, size_t *result_count)
{
    MergeNode *merge_heap = NULL;
    FrequencyNode *top_heap = NULL;
    size_t merge_size = 0U;
    size_t top_size = 0U;
    size_t i;

    if (result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0U;

    if (k == 0U || list_count == 0U) {
        return 0;
    }

    if (lists == NULL || lengths == NULL ||
        list_count > SIZE_MAX / sizeof(*merge_heap) ||
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

    for (i = 0U; i < list_count; ++i) {
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
        int value = node.value;
        size_t frequency = 0U;

        for (;;) {
            ++frequency;

            if (node.element_index + 1U < lengths[node.list_index]) {
                MergeNode next;

                next.value = lists[node.list_index][node.element_index + 1U];
                next.list_index = node.list_index;
                next.element_index = node.element_index + 1U;
                merge_heap_push(merge_heap, &merge_size, next);
            }

            if (merge_size == 0U || merge_heap[0].value != value) {
                break;
            }

            node = merge_heap_pop(merge_heap, &merge_size);
        }

        (void)consider_frequency(top_heap, &top_size, k, value, frequency);
    }

    free(merge_heap);

    qsort(top_heap, top_size, sizeof(*top_heap), result_compare);
    *result = top_heap;
    *result_count = top_size;
    return 0;
}

int main(void)
{
    static const int list0[] = { 1, 3, 5, 7, 9 };
    static const int list1[] = { 1, 2, 3, 7, 10 };
    static const int list2[] = { 1, 3, 4, 7, 11 };
    static const int list3[] = { 2, 3, 6, 7, 12 };
    static const int *const lists[] = { list0, list1, list2, list3 };
    static const size_t lengths[] = {
        sizeof(list0) / sizeof(list0[0]),
        sizeof(list1) / sizeof(list1[0]),
        sizeof(list2) / sizeof(list2[0]),
        sizeof(list3) / sizeof(list3[0])
    };
    FrequencyNode *result = NULL;
    size_t result_count = 0U;
    size_t i;
    const size_t k = 3U;

    if (find_top_k(lists, lengths,
                   sizeof(lists) / sizeof(lists[0]),
                   k, &result, &result_count) != 0) {
        fputs("Unable to find the top integers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < result_count; ++i) {
        if (printf("%d %zu\n", result[i].value,
                   result[i].frequency) < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}