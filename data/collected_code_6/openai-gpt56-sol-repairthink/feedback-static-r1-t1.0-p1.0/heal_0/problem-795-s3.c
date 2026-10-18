#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int id;
    double price;
} Item;

static int is_more_expensive(const Item *a, const Item *b)
{
    if (a->price != b->price) {
        return a->price > b->price;
    }

    return a->id > b->id;
}

static int compare_items_ascending(const void *lhs, const void *rhs)
{
    const Item *a = lhs;
    const Item *b = rhs;

    if (a->price < b->price) {
        return -1;
    }
    if (a->price > b->price) {
        return 1;
    }
    if (a->id < b->id) {
        return -1;
    }
    if (a->id > b->id) {
        return 1;
    }

    return 0;
}

static void swap_items(Item *a, Item *b)
{
    Item temporary = *a;
    *a = *b;
    *b = temporary;
}

static void sift_up(Item *heap, size_t index)
{
    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (!is_more_expensive(&heap[index], &heap[parent])) {
            break;
        }

        swap_items(&heap[index], &heap[parent]);
        index = parent;
    }
}

static void sift_down(Item *heap, size_t size, size_t index)
{
    for (;;) {
        size_t largest = index;
        size_t left;
        size_t right;

        if (index > (SIZE_MAX - 2U) / 2U) {
            break;
        }

        left = index * 2U + 1U;
        right = left + 1U;

        if (left < size && is_more_expensive(&heap[left], &heap[largest])) {
            largest = left;
        }

        if (right < size && is_more_expensive(&heap[right], &heap[largest])) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        swap_items(&heap[index], &heap[largest]);
        index = largest;
    }
}

static int find_n_cheapest(const Item *items,
                           size_t item_count,
                           size_t n,
                           Item **result,
                           size_t *result_count)
{
    Item *heap;
    size_t heap_size = 0U;
    size_t i;

    if (result == NULL || result_count == NULL) {
        return EINVAL;
    }

    *result = NULL;
    *result_count = 0U;

    if ((items == NULL && item_count != 0U) || n > item_count) {
        return EINVAL;
    }

    if (n == 0U) {
        return 0;
    }

    if (n > SIZE_MAX / sizeof(*heap)) {
        return EOVERFLOW;
    }

    heap = malloc(n * sizeof(*heap));
    if (heap == NULL) {
        return ENOMEM;
    }

    for (i = 0U; i < item_count; ++i) {
        if (heap_size < n) {
            heap[heap_size] = items[i];
            sift_up(heap, heap_size);
            ++heap_size;
        } else if (is_more_expensive(&heap[0], &items[i])) {
            heap[0] = items[i];
            sift_down(heap, heap_size, 0U);
        }
    }

    qsort(heap, heap_size, sizeof(*heap), compare_items_ascending);

    *result = heap;
    *result_count = heap_size;
    return 0;
}

int main(void)
{
    const Item items[] = {
        {101U, 19.99},
        {102U, 5.49},
        {103U, 12.75},
        {104U, 3.25},
        {105U, 8.10},
        {106U, 3.25},
        {107U, 21.00}
    };
    const size_t item_count = sizeof(items) / sizeof(items[0]);
    const size_t n = 3U;
    Item *cheapest = NULL;
    size_t cheapest_count = 0U;
    size_t i;
    int status;

    status = find_n_cheapest(items, item_count, n, &cheapest, &cheapest_count);
    if (status != 0) {
        fprintf(stderr, "Unable to find cheapest items: error %d\n", status);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < cheapest_count; ++i) {
        if (printf("ID: %u, Price: %.2f\n",
                   cheapest[i].id,
                   cheapest[i].price) < 0) {
            free(cheapest);
            return EXIT_FAILURE;
        }
    }

    free(cheapest);
    return EXIT_SUCCESS;
}