#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int id;
    double price;
} Item;

static void swap_items(Item *a, Item *b)
{
    Item temporary = *a;
    *a = *b;
    *b = temporary;
}

static void sift_up(Item heap[], size_t index)
{
    while (index > 0U) {
        size_t parent = (index - 1U) / 2U;

        if (heap[parent].price >= heap[index].price) {
            break;
        }

        swap_items(&heap[parent], &heap[index]);
        index = parent;
    }
}

static void sift_down(Item heap[], size_t size, size_t index)
{
    for (;;) {
        size_t largest = index;
        size_t left = (index * 2U) + 1U;
        size_t right = left + 1U;

        if (left < size && heap[left].price > heap[largest].price) {
            largest = left;
        }

        if (right < size && heap[right].price > heap[largest].price) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        swap_items(&heap[index], &heap[largest]);
        index = largest;
    }
}

static int compare_prices(const void *left, const void *right)
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

static int find_cheapest_items(const Item items[],
                               size_t item_count,
                               size_t n,
                               Item **result,
                               size_t *result_count)
{
    Item *heap = NULL;
    size_t heap_size = 0U;
    size_t i;

    if (result == NULL || result_count == NULL) {
        return EINVAL;
    }

    *result = NULL;
    *result_count = 0U;

    if ((items == NULL && item_count != 0U) || n == 0U) {
        return n == 0U ? 0 : EINVAL;
    }

    if (n > item_count) {
        n = item_count;
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
        if (items[i].price < 0.0) {
            free(heap);
            return EINVAL;
        }

        if (heap_size < n) {
            heap[heap_size] = items[i];
            sift_up(heap, heap_size);
            ++heap_size;
        } else if (items[i].price < heap[0].price ||
                   (items[i].price == heap[0].price &&
                    items[i].id < heap[0].id)) {
            heap[0] = items[i];
            sift_down(heap, heap_size, 0U);
        }
    }

    qsort(heap, heap_size, sizeof(*heap), compare_prices);

    *result = heap;
    *result_count = heap_size;
    return 0;
}

int main(void)
{
    const Item items[] = {
        {101U, 19.99},
        {102U, 7.50},
        {103U, 12.25},
        {104U, 3.99},
        {105U, 8.75},
        {106U, 2.49},
        {107U, 15.00}
    };
    const size_t item_count = sizeof(items) / sizeof(items[0]);
    const size_t n = 3U;
    Item *cheapest = NULL;
    size_t cheapest_count = 0U;
    size_t i;
    int status;

    status = find_cheapest_items(items, item_count, n,
                                 &cheapest, &cheapest_count);
    if (status != 0) {
        fprintf(stderr, "Unable to find cheapest items: error %d\n", status);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < cheapest_count; ++i) {
        if (printf("ID: %u, Price: %.2f\n",
                   cheapest[i].id, cheapest[i].price) < 0) {
            free(cheapest);
            return EXIT_FAILURE;
        }
    }

    free(cheapest);

    if (fclose(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}