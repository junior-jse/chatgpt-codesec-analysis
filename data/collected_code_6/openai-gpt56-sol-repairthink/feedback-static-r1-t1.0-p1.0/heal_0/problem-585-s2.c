#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *name;
    double price;
} Item;

static void swap_items(Item *a, Item *b)
{
    Item temp = *a;
    *a = *b;
    *b = temp;
}

static void sift_up(Item *heap, size_t index)
{
    while (index > 0) {
        size_t parent = (index - 1U) / 2U;

        if (heap[parent].price <= heap[index].price) {
            break;
        }

        swap_items(&heap[parent], &heap[index]);
        index = parent;
    }
}

static void sift_down(Item *heap, size_t size, size_t index)
{
    for (;;) {
        size_t smallest = index;
        size_t left = index * 2U + 1U;
        size_t right = left + 1U;

        if (left < size && heap[left].price < heap[smallest].price) {
            smallest = left;
        }

        if (right < size && heap[right].price < heap[smallest].price) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        swap_items(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

static int compare_descending(const void *left, const void *right)
{
    const Item *a = left;
    const Item *b = right;

    if (a->price < b->price) {
        return 1;
    }

    if (a->price > b->price) {
        return -1;
    }

    return 0;
}

static int find_most_expensive(const Item *items, size_t count, size_t n,
                               Item **result)
{
    Item *heap;
    size_t heap_size = 0;

    if (result == NULL || (items == NULL && count != 0U) ||
        n == 0U || n > count || n > SIZE_MAX / sizeof(*heap)) {
        errno = EINVAL;
        return -1;
    }

    *result = NULL;
    heap = malloc(n * sizeof(*heap));
    if (heap == NULL) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (heap_size < n) {
            heap[heap_size] = items[i];
            sift_up(heap, heap_size);
            ++heap_size;
        } else if (items[i].price > heap[0].price) {
            heap[0] = items[i];
            sift_down(heap, heap_size, 0);
        }
    }

    qsort(heap, heap_size, sizeof(*heap), compare_descending);
    *result = heap;
    return 0;
}

int main(void)
{
    Item items[] = {
        {"Laptop", 1299.99},
        {"Phone", 899.50},
        {"Monitor", 449.00},
        {"Keyboard", 129.99},
        {"Tablet", 749.25},
        {"Camera", 1099.00},
        {"Headphones", 299.95}
    };
    const size_t item_count = sizeof(items) / sizeof(items[0]);
    const size_t n = 3;
    Item *expensive_items = NULL;

    if (find_most_expensive(items, item_count, n, &expensive_items) != 0) {
        perror("find_most_expensive");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < n; ++i) {
        if (printf("%s: %.2f\n", expensive_items[i].name,
                   expensive_items[i].price) < 0) {
            free(expensive_items);
            return EXIT_FAILURE;
        }
    }

    free(expensive_items);
    return EXIT_SUCCESS;
}