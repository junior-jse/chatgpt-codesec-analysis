#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const int *values;
    size_t length;
    size_t position;
} SortedInput;

typedef struct {
    int value;
    size_t input_index;
} HeapNode;

typedef struct {
    SortedInput *inputs;
    size_t input_count;
    HeapNode *heap;
    size_t heap_size;
} MergeIterator;

static bool heap_node_less(const HeapNode *left, const HeapNode *right)
{
    if (left->value != right->value) {
        return left->value < right->value;
    }

    return left->input_index < right->input_index;
}

static void heap_swap(HeapNode *left, HeapNode *right)
{
    HeapNode temporary = *left;
    *left = *right;
    *right = temporary;
}

static void heap_sift_up(HeapNode *heap, size_t index)
{
    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!heap_node_less(&heap[index], &heap[parent])) {
            break;
        }

        heap_swap(&heap[index], &heap[parent]);
        index = parent;
    }
}

static void heap_sift_down(HeapNode *heap, size_t heap_size, size_t index)
{
    for (;;) {
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;
        size_t smallest = index;

        if (left < heap_size && heap_node_less(&heap[left], &heap[smallest])) {
            smallest = left;
        }

        if (right < heap_size && heap_node_less(&heap[right], &heap[smallest])) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        heap_swap(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

static bool heap_push(MergeIterator *iterator, HeapNode node)
{
    if (iterator == NULL ||
        iterator->heap == NULL ||
        iterator->heap_size >= iterator->input_count) {
        return false;
    }

    iterator->heap[iterator->heap_size] = node;
    heap_sift_up(iterator->heap, iterator->heap_size);
    ++iterator->heap_size;
    return true;
}

static bool merge_iterator_init(
    MergeIterator *iterator,
    const SortedInput *inputs,
    size_t input_count)
{
    size_t index;

    if (iterator == NULL || (input_count > 0U && inputs == NULL)) {
        return false;
    }

    iterator->inputs = NULL;
    iterator->input_count = 0U;
    iterator->heap = NULL;
    iterator->heap_size = 0U;

    if (input_count == 0U) {
        return true;
    }

    if (input_count > SIZE_MAX / sizeof(*iterator->inputs) ||
        input_count > SIZE_MAX / sizeof(*iterator->heap)) {
        return false;
    }

    iterator->inputs = malloc(input_count * sizeof(*iterator->inputs));
    iterator->heap = malloc(input_count * sizeof(*iterator->heap));

    if (iterator->inputs == NULL || iterator->heap == NULL) {
        free(iterator->heap);
        free(iterator->inputs);
        iterator->heap = NULL;
        iterator->inputs = NULL;
        return false;
    }

    iterator->input_count = input_count;

    for (index = 0U; index < input_count; ++index) {
        if (inputs[index].length > 0U && inputs[index].values == NULL) {
            free(iterator->heap);
            free(iterator->inputs);
            iterator->heap = NULL;
            iterator->inputs = NULL;
            iterator->input_count = 0U;
            return false;
        }

        iterator->inputs[index] = inputs[index];
        iterator->inputs[index].position = 0U;

        if (inputs[index].length > 0U) {
            HeapNode node = {
                .value = inputs[index].values[0],
                .input_index = index
            };

            if (!heap_push(iterator, node)) {
                free(iterator->heap);
                free(iterator->inputs);
                iterator->heap = NULL;
                iterator->inputs = NULL;
                iterator->input_count = 0U;
                iterator->heap_size = 0U;
                return false;
            }
        }
    }

    return true;
}

static bool merge_iterator_next(MergeIterator *iterator, int *value)
{
    HeapNode root;
    SortedInput *input;

    if (iterator == NULL || value == NULL || iterator->heap_size == 0U) {
        return false;
    }

    root = iterator->heap[0];
    *value = root.value;
    input = &iterator->inputs[root.input_index];
    ++input->position;

    if (input->position < input->length) {
        iterator->heap[0].value = input->values[input->position];
        iterator->heap[0].input_index = root.input_index;
    } else {
        --iterator->heap_size;

        if (iterator->heap_size > 0U) {
            iterator->heap[0] = iterator->heap[iterator->heap_size];
        }
    }

    if (iterator->heap_size > 0U) {
        heap_sift_down(iterator->heap, iterator->heap_size, 0U);
    }

    return true;
}

static void merge_iterator_destroy(MergeIterator *iterator)
{
    if (iterator == NULL) {
        return;
    }

    free(iterator->heap);
    free(iterator->inputs);
    iterator->heap = NULL;
    iterator->inputs = NULL;
    iterator->heap_size = 0U;
    iterator->input_count = 0U;
}

int main(void)
{
    static const int first[] = {1, 4, 7, 10};
    static const int second[] = {2, 5, 8, 11};
    static const int third[] = {3, 6, 9, 12};
    SortedInput inputs[] = {
        {first, sizeof(first) / sizeof(first[0]), 0U},
        {second, sizeof(second) / sizeof(second[0]), 0U},
        {third, sizeof(third) / sizeof(third[0]), 0U}
    };
    MergeIterator iterator;
    int value;
    bool first_output = true;

    if (!merge_iterator_init(
            &iterator,
            inputs,
            sizeof(inputs) / sizeof(inputs[0]))) {
        fputs("Failed to initialize merge iterator\n", stderr);
        return EXIT_FAILURE;
    }

    while (merge_iterator_next(&iterator, &value)) {
        if (!first_output && putchar(' ') == EOF) {
            merge_iterator_destroy(&iterator);
            return EXIT_FAILURE;
        }

        if (printf("%d", value) < 0) {
            merge_iterator_destroy(&iterator);
            return EXIT_FAILURE;
        }

        first_output = false;
    }

    if (putchar('\n') == EOF) {
        merge_iterator_destroy(&iterator);
        return EXIT_FAILURE;
    }

    merge_iterator_destroy(&iterator);
    return EXIT_SUCCESS;
}