#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[64];
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
    if (heap == NULL || capacity == 0) {
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
        {1, "Laptop",       1299.99},
        {2, "Smartphone",    899.50},
        {3, "Tablet",        549.00},
        {4, "Headphones",    349.99},
        {5, "Monitor",       799.00},
        {6, "Keyboard",      149.99},
        {7, "Mouse",          79.99},
        {8, "Printer",       449.00},
        {9, "Camera",       1099.00},
        {10, "Speaker",      259.99}
    };

    size_t dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    size_t n = 3;
    Item *result;
    int found;
    int i;

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