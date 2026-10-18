#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int value;
    size_t list_index;
    size_t element_index;
} MergeNode;

typedef struct {
    int value;
    size_t frequency;
} FrequencyNode;

static bool merge_less(const MergeNode *a, const MergeNode *b)
{
    return a->value < b->value;
}

static bool frequency_less(const FrequencyNode *a, const FrequencyNode *b)
{
    if (a->frequency != b->frequency) {
        return a->frequency < b->frequency;
    }
    return a->value > b->value;
}

static void merge_swap(MergeNode *a, MergeNode *b)
{
    MergeNode temporary = *a;
    *a = *b;
    *b = temporary;
}

static void frequency_swap(FrequencyNode *a, FrequencyNode *b)
{
    FrequencyNode temporary = *a;
    *a = *b;
    *b = temporary;
}

static void merge_heap_push(MergeNode *heap, size_t *size, MergeNode node)
{
    size_t index = (*size)++;
    heap[index] = node;

    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!merge_less(&heap[index], &heap[parent])) {
            break;
        }

        merge_swap(&heap[index], &heap[parent]);
        index = parent;
    }
}

static MergeNode merge_heap_pop(MergeNode *heap, size_t *size)
{
    MergeNode result = heap[0];
    heap[0] = heap[--(*size)];

    size_t index = 0U;

    for (;;) {
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;
        size_t smallest = index;

        if (left < *size && merge_less(&heap[left], &heap[smallest])) {
            smallest = left;
        }

        if (right < *size && merge_less(&heap[right], &heap[smallest])) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        merge_swap(&heap[index], &heap[smallest]);
        index = smallest;
    }

    return result;
}

static void frequency_heap_push(
    FrequencyNode *heap,
    size_t *size,
    FrequencyNode node)
{
    size_t index = (*size)++;
    heap[index] = node;

    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!frequency_less(&heap[index], &heap[parent])) {
            break;
        }

        frequency_swap(&heap[index], &heap[parent]);
        index = parent;
    }
}

static void frequency_heap_replace_root(
    FrequencyNode *heap,
    size_t size,
    FrequencyNode node)
{
    heap[0] = node;
    size_t index = 0U;

    for (;;) {
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;
        size_t smallest = index;

        if (left < size && frequency_less(&heap[left], &heap[smallest])) {
            smallest = left;
        }

        if (right < size && frequency_less(&heap[right], &heap[smallest])) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        frequency_swap(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

static int output_compare(const void *left, const void *right)
{
    const FrequencyNode *a = left;
    const FrequencyNode *b = right;

    if (a->frequency < b->frequency) {
        return 1;
    }

    if (a->frequency > b->frequency) {
        return -1;
    }

    if (a->value > b->value) {
        return 1;
    }

    if (a->value < b->value) {
        return -1;
    }

    return 0;
}

static bool consider_frequency(
    FrequencyNode *top_heap,
    size_t *top_size,
    size_t k,
    int value,
    size_t frequency)
{
    FrequencyNode candidate = { value, frequency };

    if (*top_size < k) {
        frequency_heap_push(top_heap, top_size, candidate);
        return true;
    }

    if (frequency_less(&top_heap[0], &candidate)) {
        frequency_heap_replace_root(top_heap, *top_size, candidate);
    }

    return true;
}

bool find_top_k_frequent(
    const int *const lists[],
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
    bool have_current = false;
    int current_value = 0;
    size_t current_frequency = 0U;

    if (result == NULL || result_count == NULL) {
        return false;
    }

    *result = NULL;
    *result_count = 0U;

    if (k == 0U || list_count == 0U) {
        return true;
    }

    if (lists == NULL || lengths == NULL) {
        return false;
    }

    if (list_count > SIZE_MAX / sizeof(*merge_heap) ||
        k > SIZE_MAX / sizeof(*top_heap)) {
        return false;
    }

    merge_heap = malloc(list_count * sizeof(*merge_heap));
    top_heap = malloc(k * sizeof(*top_heap));

    if (merge_heap == NULL || top_heap == NULL) {
        free(merge_heap);
        free(top_heap);
        return false;
    }

    for (size_t i = 0U; i < list_count; ++i) {
        if (lengths[i] != 0U) {
            if (lists[i] == NULL) {
                free(merge_heap);
                free(top_heap);
                return false;
            }

            MergeNode node = { lists[i][0], i, 0U };
            merge_heap_push(merge_heap, &merge_size, node);
        }
    }

    while (merge_size > 0U) {
        MergeNode node = merge_heap_pop(merge_heap, &merge_size);

        if (!have_current || node.value != current_value) {
            if (have_current) {
                consider_frequency(
                    top_heap,
                    &top_size,
                    k,
                    current_value,
                    current_frequency);
            }

            current_value = node.value;
            current_frequency = 1U;
            have_current = true;
        } else {
            ++current_frequency;
        }

        if (node.element_index + 1U < lengths[node.list_index]) {
            ++node.element_index;
            node.value = lists[node.list_index][node.element_index];
            merge_heap_push(merge_heap, &merge_size, node);
        }
    }

    if (have_current) {
        consider_frequency(
            top_heap,
            &top_size,
            k,
            current_value,
            current_frequency);
    }

    free(merge_heap);

    qsort(top_heap, top_size, sizeof(*top_heap), output_compare);

    if (top_size == 0U) {
        free(top_heap);
        return true;
    }

    FrequencyNode *shrunk = realloc(top_heap, top_size * sizeof(*top_heap));
    if (shrunk != NULL) {
        top_heap = shrunk;
    }

    *result = top_heap;
    *result_count = top_size;
    return true;
}

int main(void)
{
    static const int list1[] = { 1, 2, 3, 5, 8 };
    static const int list2[] = { 1, 2, 4, 5, 8 };
    static const int list3[] = { 1, 3, 5, 7, 8 };
    static const int list4[] = { 2, 5, 6, 8, 9 };

    const int *lists[] = { list1, list2, list3, list4 };
    const size_t lengths[] = {
        sizeof(list1) / sizeof(list1[0]),
        sizeof(list2) / sizeof(list2[0]),
        sizeof(list3) / sizeof(list3[0]),
        sizeof(list4) / sizeof(list4[0])
    };

    FrequencyNode *result = NULL;
    size_t result_count = 0U;
    const size_t k = 3U;

    if (!find_top_k_frequent(
            lists,
            lengths,
            sizeof(lists) / sizeof(lists[0]),
            k,
            &result,
            &result_count)) {
        fputs("Unable to find the top frequent integers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < result_count; ++i) {
        printf("%d %zu\n", result[i].value, result[i].frequency);
    }

    free(result);
    return EXIT_SUCCESS;
}