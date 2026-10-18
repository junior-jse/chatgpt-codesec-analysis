#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int id;
    const char *name;
    double price;
} Item;

static bool is_more_expensive(const Item *left, const Item *right)
{
    if (left->price != right->price) {
        return left->price > right->price;
    }

    return left->id > right->id;
}

static void swap_items(Item *left, Item *right)
{
    Item temporary = *left;
    *left = *right;
    *right = temporary;
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

static void sift_down(Item *heap, size_t count, size_t index)
{
    for (;;) {
        size_t largest = index;
        size_t left;
        size_t right;

        if (index > (SIZE_MAX - 2U) / 2U) {
            break;
        }

        left = (index * 2U) + 1U;
        right = left + 1U;

        if (left < count && is_more_expensive(&heap[left], &heap[largest])) {
            largest = left;
        }

        if (right < count && is_more_expensive(&heap[right], &heap[largest])) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        swap_items(&heap[index], &heap[largest]);
        index = largest;
    }
}

static int compare_items(const void *left, const void *right)
{
    const Item *a = left;
    const Item *b = right;

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

static int find_n_cheapest(const Item *items, size_t item_count,
                           size_t n, Item **result, size_t *result_count)
{
    Item *heap = NULL;
    size_t heap_count = 0U;
    size_t i;

    if (result == NULL || result_count == NULL) {
        return EINVAL;
    }

    *result = NULL;
    *result_count = 0U;

    if (n == 0U || item_count == 0U) {
        return 0;
    }

    if (items == NULL) {
        return EINVAL;
    }

    if (n > item_count) {
        n = item_count;
    }

    if (n > SIZE_MAX / sizeof(*heap)) {
        return EOVERFLOW;
    }

    heap = malloc(n * sizeof(*heap));
    if (heap == NULL) {
        return ENOMEM;
    }

    for (i = 0U; i < item_count; ++i) {
        if (items[i].name == NULL || items[i].price < 0.0 ||
            items[i].price != items[i].price) {
            free(heap);
            return EINVAL;
        }

        if (heap_count < n) {
            heap[heap_count] = items[i];
            sift_up(heap, heap_count);
            ++heap_count;
        } else if (is_more_expensive(&heap[0], &items[i])) {
            heap[0] = items[i];
            sift_down(heap, heap_count, 0U);
        }
    }

    qsort(heap, heap_count, sizeof(*heap), compare_items);
    *result = heap;
    *result_count = heap_count;

    return 0;
}

int main(void)
{
    const Item items[] = {
        {101U, "Keyboard", 49.99},
        {102U, "Mouse", 19.95},
        {103U, "Monitor", 179.50},
        {104U, "USB Cable", 7.25},
        {105U, "Webcam", 35.00},
        {106U, "Headphones", 24.75},
        {107U, "Laptop Stand", 29.99}
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
        if (printf("%u\t%s\t%.2f\n",
                   cheapest[i].id,
                   cheapest[i].name,
                   cheapest[i].price) < 0) {
            free(cheapest);
            return EXIT_FAILURE;
        }
    }

    free(cheapest);

    if (fflush(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}