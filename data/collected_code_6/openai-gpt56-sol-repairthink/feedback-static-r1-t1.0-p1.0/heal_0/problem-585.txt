#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    double price;
} Item;

static int item_is_less(const Item *a, const Item *b)
{
    if (a->price < b->price) {
        return 1;
    }
    if (a->price > b->price) {
        return 0;
    }
    return strcmp(a->name, b->name) > 0;
}

static int item_is_more(const Item *a, const Item *b)
{
    if (a->price > b->price) {
        return 1;
    }
    if (a->price < b->price) {
        return 0;
    }
    return strcmp(a->name, b->name) < 0;
}

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

        if (!item_is_less(&heap[index], &heap[parent])) {
            break;
        }

        swap_items(&heap[index], &heap[parent]);
        index = parent;
    }
}

static void sift_down(Item heap[], size_t size, size_t index)
{
    for (;;) {
        size_t left = index * 2U + 1U;
        size_t right;
        size_t smallest = index;

        if (left >= size) {
            break;
        }

        right = left + 1U;

        if (item_is_less(&heap[left], &heap[smallest])) {
            smallest = left;
        }

        if (right < size && item_is_less(&heap[right], &heap[smallest])) {
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

    if (item_is_more(a, b)) {
        return -1;
    }
    if (item_is_more(b, a)) {
        return 1;
    }
    return 0;
}

static int find_most_expensive(const Item items[], size_t item_count,
                               size_t n, Item output[])
{
    size_t heap_size = 0U;

    if ((items == NULL && item_count != 0U) ||
        (output == NULL && n != 0U) || n > item_count) {
        errno = EINVAL;
        return -1;
    }

    if (n == 0U) {
        return 0;
    }

    for (size_t i = 0U; i < item_count; ++i) {
        if (items[i].name == NULL || items[i].price < 0.0) {
            errno = EINVAL;
            return -1;
        }

        if (heap_size < n) {
            output[heap_size] = items[i];
            sift_up(output, heap_size);
            ++heap_size;
        } else if (item_is_more(&items[i], &output[0])) {
            output[0] = items[i];
            sift_down(output, heap_size, 0U);
        }
    }

    qsort(output, n, sizeof(*output), compare_descending);
    return 0;
}

int main(void)
{
    Item items[] = {
        {"Laptop", 1299.99},
        {"Phone", 899.50},
        {"Monitor", 449.00},
        {"Keyboard", 129.95},
        {"Workstation", 3499.00},
        {"Tablet", 749.99},
        {"Server", 5999.95}
    };
    const size_t item_count = sizeof(items) / sizeof(items[0]);
    const size_t n = 3U;
    Item *result;

    if (n > item_count || n > SIZE_MAX / sizeof(*result)) {
        fputs("Invalid result size\n", stderr);
        return EXIT_FAILURE;
    }

    result = malloc(n * sizeof(*result));
    if (result == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    if (find_most_expensive(items, item_count, n, result) != 0) {
        perror("find_most_expensive");
        free(result);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < n; ++i) {
        if (printf("%s: %.2f\n", result[i].name, result[i].price) < 0) {
            fputs("Output error\n", stderr);
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);

    if (fflush(stdout) == EOF) {
        perror("fflush");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}