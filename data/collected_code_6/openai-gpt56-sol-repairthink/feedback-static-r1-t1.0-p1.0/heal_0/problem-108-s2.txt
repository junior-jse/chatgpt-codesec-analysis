#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

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

static void swap_nodes(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static int node_less(const HeapNode *a, const HeapNode *b)
{
    if (a->value != b->value) {
        return a->value < b->value;
    }
    if (a->input_index != b->input_index) {
        return a->input_index < b->input_index;
    }
    return a->element_index < b->element_index;
}

static void heap_sift_up(HeapNode *heap, size_t index)
{
    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!node_less(&heap[index], &heap[parent])) {
            break;
        }

        swap_nodes(&heap[index], &heap[parent]);
        index = parent;
    }
}

static void heap_sift_down(HeapNode *heap, size_t heap_size, size_t index)
{
    for (;;) {
        size_t left;
        size_t right;
        size_t smallest;

        if (index > (SIZE_MAX - 1U) / 2U) {
            break;
        }

        left = index * 2U + 1U;
        if (left >= heap_size) {
            break;
        }

        right = left + 1U;
        smallest = left;

        if (right < heap_size && node_less(&heap[right], &heap[left])) {
            smallest = right;
        }

        if (!node_less(&heap[smallest], &heap[index])) {
            break;
        }

        swap_nodes(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

static int merge_iterator_init(
    MergeIterator *iterator,
    const SortedInput *inputs,
    size_t input_count)
{
    size_t i;

    if (iterator == NULL || (inputs == NULL && input_count != 0U)) {
        return 0;
    }

    iterator->inputs = inputs;
    iterator->input_count = input_count;
    iterator->heap = NULL;
    iterator->heap_size = 0U;

    if (input_count == 0U) {
        return 1;
    }

    if (input_count > SIZE_MAX / sizeof(*iterator->heap)) {
        return 0;
    }

    iterator->heap = malloc(input_count * sizeof(*iterator->heap));
    if (iterator->heap == NULL) {
        return 0;
    }

    for (i = 0U; i < input_count; ++i) {
        HeapNode node;

        if (inputs[i].values == NULL && inputs[i].length != 0U) {
            free(iterator->heap);
            iterator->heap = NULL;
            return 0;
        }

        if (inputs[i].length == 0U) {
            continue;
        }

        node.value = inputs[i].values[0];
        node.input_index = i;
        node.element_index = 0U;
        iterator->heap[iterator->heap_size] = node;
        heap_sift_up(iterator->heap, iterator->heap_size);
        ++iterator->heap_size;
    }

    return 1;
}

static int merge_iterator_next(MergeIterator *iterator, int *value)
{
    HeapNode node;
    const SortedInput *input;
    size_t next_index;

    if (iterator == NULL || value == NULL || iterator->heap_size == 0U) {
        return 0;
    }

    node = iterator->heap[0];
    *value = node.value;
    input = &iterator->inputs[node.input_index];
    next_index = node.element_index + 1U;

    if (next_index < input->length) {
        iterator->heap[0].value = input->values[next_index];
        iterator->heap[0].input_index = node.input_index;
        iterator->heap[0].element_index = next_index;
    } else {
        --iterator->heap_size;
        if (iterator->heap_size != 0U) {
            iterator->heap[0] = iterator->heap[iterator->heap_size];
        }
    }

    if (iterator->heap_size != 0U) {
        heap_sift_down(iterator->heap, iterator->heap_size, 0U);
    }

    return 1;
}

static void merge_iterator_destroy(MergeIterator *iterator)
{
    if (iterator == NULL) {
        return;
    }

    free(iterator->heap);
    iterator->inputs = NULL;
    iterator->input_count = 0U;
    iterator->heap = NULL;
    iterator->heap_size = 0U;
}

int main(void)
{
    static const int first[] = {1, 4, 7, 10};
    static const int second[] = {2, 5, 8, 11};
    static const int third[] = {3, 6, 9, 12};
    static const SortedInput inputs[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    MergeIterator iterator;
    int value;
    int first_output = 1;

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

        first_output = 0;
    }

    if (putchar('\n') == EOF) {
        merge_iterator_destroy(&iterator);
        return EXIT_FAILURE;
    }

    merge_iterator_destroy(&iterator);
    return EXIT_SUCCESS;
}