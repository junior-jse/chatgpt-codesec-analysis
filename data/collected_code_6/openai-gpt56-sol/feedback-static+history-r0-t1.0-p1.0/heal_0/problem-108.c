#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    const int *values;
    size_t length;
} SortedInput;

typedef struct {
    int value;
    size_t input_index;
    size_t element_index;
} HeapNode;

typedef struct {
    const SortedInput *inputs;
    size_t input_count;
    HeapNode *heap;
    size_t heap_size;
} MergeIterator;

static bool size_multiply_overflows(size_t count, size_t size)
{
    return size != 0U && count > SIZE_MAX / size;
}

static bool node_less(const HeapNode *left, const HeapNode *right)
{
    if (left->value != right->value) {
        return left->value < right->value;
    }
    if (left->input_index != right->input_index) {
        return left->input_index < right->input_index;
    }
    return left->element_index < right->element_index;
}

static void heap_swap(HeapNode *left, HeapNode *right)
{
    HeapNode temporary = *left;
    *left = *right;
    *right = temporary;
}

static void heap_push(MergeIterator *iterator, HeapNode node)
{
    size_t index = iterator->heap_size++;

    iterator->heap[index] = node;

    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!node_less(&iterator->heap[index], &iterator->heap[parent])) {
            break;
        }

        heap_swap(&iterator->heap[index], &iterator->heap[parent]);
        index = parent;
    }
}

static HeapNode heap_pop(MergeIterator *iterator)
{
    HeapNode result = iterator->heap[0];
    size_t index = 0U;

    --iterator->heap_size;

    if (iterator->heap_size == 0U) {
        return result;
    }

    iterator->heap[0] = iterator->heap[iterator->heap_size];

    for (;;) {
        size_t left;
        size_t right;
        size_t smallest;

        if (index > (iterator->heap_size - 1U) / 2U) {
            break;
        }

        left = index * 2U + 1U;
        if (left >= iterator->heap_size) {
            break;
        }

        right = left + 1U;
        smallest = left;

        if (right < iterator->heap_size &&
            node_less(&iterator->heap[right], &iterator->heap[left])) {
            smallest = right;
        }

        if (!node_less(&iterator->heap[smallest], &iterator->heap[index])) {
            break;
        }

        heap_swap(&iterator->heap[index], &iterator->heap[smallest]);
        index = smallest;
    }

    return result;
}

static bool merge_iterator_init(
    MergeIterator *iterator,
    const SortedInput *inputs,
    size_t input_count)
{
    size_t i;

    if (iterator == NULL || (inputs == NULL && input_count != 0U)) {
        return false;
    }

    iterator->inputs = inputs;
    iterator->input_count = input_count;
    iterator->heap = NULL;
    iterator->heap_size = 0U;

    for (i = 0U; i < input_count; ++i) {
        if (inputs[i].values == NULL && inputs[i].length != 0U) {
            iterator->inputs = NULL;
            iterator->input_count = 0U;
            return false;
        }
    }

    if (input_count == 0U) {
        return true;
    }

    if (size_multiply_overflows(input_count, sizeof(*iterator->heap))) {
        iterator->inputs = NULL;
        iterator->input_count = 0U;
        return false;
    }

    iterator->heap = malloc(input_count * sizeof(*iterator->heap));
    if (iterator->heap == NULL) {
        iterator->inputs = NULL;
        iterator->input_count = 0U;
        return false;
    }

    for (i = 0U; i < input_count; ++i) {
        if (inputs[i].length != 0U) {
            HeapNode node = {
                inputs[i].values[0],
                i,
                0U
            };
            heap_push(iterator, node);
        }
    }

    return true;
}

static bool merge_iterator_next(MergeIterator *iterator, int *value)
{
    HeapNode node;
    const SortedInput *input;
    size_t next_index;

    if (iterator == NULL || value == NULL || iterator->heap_size == 0U) {
        return false;
    }

    node = heap_pop(iterator);
    *value = node.value;
    input = &iterator->inputs[node.input_index];
    next_index = node.element_index + 1U;

    if (next_index < input->length) {
        HeapNode next_node = {
            input->values[next_index],
            node.input_index,
            next_index
        };
        heap_push(iterator, next_node);
    }

    return true;
}

static void merge_iterator_destroy(MergeIterator *iterator)
{
    if (iterator != NULL) {
        free(iterator->heap);
        iterator->inputs = NULL;
        iterator->input_count = 0U;
        iterator->heap = NULL;
        iterator->heap_size = 0U;
    }
}

int main(void)
{
    const int first[] = {1, 4, 7, 10};
    const int second[] = {2, 5, 8, 11};
    const int third[] = {0, 3, 6, 9, 12};
    const SortedInput inputs[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
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

    if (fflush(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}