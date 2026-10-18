#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME_LEN 63

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char name[MAX_NAME_LEN + 1];
    double price;
} Item;

typedef struct {
    Item *data;
    size_t size;
    size_t capacity;
} MinHeap;

static void swap_items(Item *a, Item *b)
{
    Item temp = *a;
    *a = *b;
    *b = temp;
}

static int heap_init(MinHeap *heap, size_t capacity)
{
    if (heap == NULL || capacity == 0 || capacity > (SIZE_MAX / sizeof(Item))) {
        return -1;
    }

    heap->data = (Item *)malloc(capacity * sizeof(Item));
    if (heap->data == NULL) {
        return -1;
    }

    heap->size = 0;
    heap->capacity = capacity;
    return 0;
}

static void heap_destroy(MinHeap *heap)
{
    if (heap != NULL) {
        free(heap->data);
        heap->data = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static void heapify_up(MinHeap *heap, size_t index)
{
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap->data[parent].price <= heap->data[index].price) {
            break;
        }
        swap_items(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static void heapify_down(MinHeap *heap, size_t index)
{
    for (;;) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t smallest = index;

        if (left < heap->size &&
            heap->data[left].price < heap->data[smallest].price) {
            smallest = left;
        }

        if (right < heap->size &&
            heap->data[right].price < heap->data[smallest].price) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        swap_items(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

static int heap_push(MinHeap *heap, const Item *item)
{
    if (heap == NULL || item == NULL || heap->size >= heap->capacity) {
        return -1;
    }

    heap->data[heap->size] = *item;
    heapify_up(heap, heap->size);
    heap->size++;
    return 0;
}

static int heap_pop(MinHeap *heap, Item *out)
{
    if (heap == NULL || out == NULL || heap->size == 0) {
        return -1;
    }

    *out = heap->data[0];
    heap->size--;

    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0);
    }

    return 0;
}

static const Item *heap_peek(const MinHeap *heap)
{
    if (heap == NULL || heap->size == 0) {
        return NULL;
    }
    return &heap->data[0];
}

static size_t safe_strnlen(const char *s, size_t maxlen)
{
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
}

static int safe_copy_name(char *dest, size_t dest_size, const char *src)
{
    size_t src_len;
    size_t copy_len;
    
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }
    
    src_len = safe_strnlen(src, dest_size);
    copy_len = (src_len >= dest_size) ? (dest_size - 1) : src_len;
    
    if (copy_len > 0) {
        if (copy_len >= dest_size) {
            return -1;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, src, copy_len);
    }
    dest[copy_len] = '\0';
    
    return 0;
}

int find_n_expensive(const Item *dataset, size_t dataset_size,
                     size_t n, Item *result)
{
    MinHeap heap;
    size_t i;
    size_t count;

    if (dataset == NULL || result == NULL || n == 0) {
        return -1;
    }

    if (n > dataset_size) {
        n = dataset_size;
    }

    if (heap_init(&heap, n) != 0) {
        return -1;
    }

    for (i = 0; i < dataset_size; i++) {
        if (heap.size < n) {
            if (heap_push(&heap, &dataset[i]) != 0) {
                heap_destroy(&heap);
                return -1;
            }
        } else {
            const Item *top = heap_peek(&heap);
            if (top != NULL && dataset[i].price > top->price) {
                Item discarded;
                if (heap_pop(&heap, &discarded) != 0) {
                    heap_destroy(&heap);
                    return -1;
                }
                if (heap_push(&heap, &dataset[i]) != 0) {
                    heap_destroy(&heap);
                    return -1;
                }
            }
        }
    }

    count = heap.size;
    for (i = 0; i < count; i++) {
        if (heap_pop(&heap, &result[i]) != 0) {
            heap_destroy(&heap);
            return -1;
        }
    }

    heap_destroy(&heap);
    return (int)count;
}

static int compare_items_desc(const void *a, const void *b)
{
    const Item *ia = (const Item *)a;
    const Item *ib = (const Item *)b;

    if (ia->price < ib->price) {
        return 1;
    }
    if (ia->price > ib->price) {
        return -1;
    }
    return 0;
}

int main(void)
{
    Item dataset[] = {
        {1, "", 1299.99},
        {2, "", 899.50},
        {3, "", 549.00},
        {4, "", 349.99},
        {5, "", 799.00},
        {6, "", 149.99},
        {7, "", 79.99},
        {8, "", 449.00},
        {9, "", 1099.00},
        {10, "", 259.99}
    };

    size_t dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    size_t n = 3;
    Item *result;
    int found;
    int i;

    if (safe_copy_name(dataset[0].name, sizeof(dataset[0].name), "Laptop") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[1].name, sizeof(dataset[1].name), "Smartphone") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[2].name, sizeof(dataset[2].name), "Tablet") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[3].name, sizeof(dataset[3].name), "Headphones") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[4].name, sizeof(dataset[4].name), "Monitor") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[5].name, sizeof(dataset[5].name), "Keyboard") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[6].name, sizeof(dataset[6].name), "Mouse") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[7].name, sizeof(dataset[7].name), "Printer") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[8].name, sizeof(dataset[8].name), "Camera") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }
    if (safe_copy_name(dataset[9].name, sizeof(dataset[9].name), "Speaker") != 0) {
        fprintf(stderr, "Failed to copy name\n");
        return EXIT_FAILURE;
    }

    result = (Item *)malloc(n * sizeof(Item));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    found = find_n_expensive(dataset, dataset_size, n, result);
    if (found < 0) {
        fprintf(stderr, "Failed to find top %zu expensive items\n", n);
        free(result);
        return EXIT_FAILURE;
    }

    qsort(result, (size_t)found, sizeof(Item), compare_items_desc);

    printf("Top %d most expensive items:\n", found);
    printf("%-5s %-15s %s\n", "ID", "Name", "Price");
    printf("%-5s %-15s %s\n", "--", "----", "-----");

    for (i = 0; i < found; i++) {
        printf("%-5d %-15s $%.2f\n",
               result[i].id, result[i].name, result[i].price);
    }

    free(result);
    return EXIT_SUCCESS;
}