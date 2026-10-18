#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    double price;
} Item;

typedef struct {
    Item *data;
    size_t size;
    size_t capacity;
} Heap;

static void swap_items(Item *a, Item *b)
{
    Item temp = *a;
    *a = *b;
    *b = temp;
}

static int heap_init(Heap *heap, size_t capacity)
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

static void heap_free(Heap *heap)
{
    if (heap != NULL) {
        free(heap->data);
        heap->data = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static void heapify_down(Heap *heap, size_t index)
{
    size_t largest = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (left < heap->size && heap->data[left].price > heap->data[largest].price) {
        largest = left;
    }

    if (right < heap->size && heap->data[right].price > heap->data[largest].price) {
        largest = right;
    }

    if (largest != index) {
        swap_items(&heap->data[index], &heap->data[largest]);
        heapify_down(heap, largest);
    }
}

static void heapify_up(Heap *heap, size_t index)
{
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap->data[parent].price >= heap->data[index].price) {
            break;
        }
        swap_items(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static int heap_push(Heap *heap, Item item)
{
    if (heap == NULL || heap->size >= heap->capacity) {
        return -1;
    }

    heap->data[heap->size] = item;
    heapify_up(heap, heap->size);
    heap->size++;
    return 0;
}

static int heap_pop(Heap *heap, Item *result)
{
    if (heap == NULL || result == NULL || heap->size == 0) {
        return -1;
    }

    *result = heap->data[0];
    heap->size--;

    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0);
    }

    return 0;
}

static Item heap_top(const Heap *heap)
{
    Item empty = { -1, 0.0 };
    if (heap == NULL || heap->size == 0) {
        return empty;
    }
    return heap->data[0];
}

int find_n_cheapest(const Item *dataset, size_t dataset_size, size_t n, Item *result)
{
    Heap heap;
    size_t i;
    size_t result_count;

    if (dataset == NULL || result == NULL || n == 0 || dataset_size == 0) {
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
            if (heap_push(&heap, dataset[i]) != 0) {
                heap_free(&heap);
                return -1;
            }
        } else {
            Item top = heap_top(&heap);
            if (dataset[i].price < top.price) {
                Item popped;
                if (heap_pop(&heap, &popped) != 0) {
                    heap_free(&heap);
                    return -1;
                }
                if (heap_push(&heap, dataset[i]) != 0) {
                    heap_free(&heap);
                    return -1;
                }
            }
        }
    }

    result_count = heap.size;
    for (i = 0; i < result_count; i++) {
        if (heap_pop(&heap, &result[i]) != 0) {
            heap_free(&heap);
            return -1;
        }
    }

    heap_free(&heap);
    return (int)result_count;
}

static int compare_items(const void *a, const void *b)
{
    const Item *ia = (const Item *)a;
    const Item *ib = (const Item *)b;

    if (ia->price < ib->price) {
        return -1;
    }
    if (ia->price > ib->price) {
        return 1;
    }
    return 0;
}

int main(void)
{
    Item dataset[] = {
        { 1, 25.50 },
        { 2, 10.99 },
        { 3, 45.00 },
        { 4, 5.75 },
        { 5, 30.20 },
        { 6, 15.00 },
        { 7, 8.49 },
        { 8, 50.00 },
        { 9, 12.30 },
        { 10, 3.99 }
    };
    size_t dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    size_t n = 4;
    Item *result = NULL;
    int count;
    int i;

    result = (Item *)malloc(n * sizeof(Item));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    count = find_n_cheapest(dataset, dataset_size, n, result);
    if (count < 0) {
        fprintf(stderr, "Failed to find cheapest items\n");
        free(result);
        return EXIT_FAILURE;
    }

    qsort(result, (size_t)count, sizeof(Item), compare_items);

    printf("The %d cheapest items:\n", count);
    printf("%-6s %s\n", "ID", "Price");
    printf("----------\n");
    for (i = 0; i < count; i++) {
        printf("%-6d %.2f\n", result[i].id, result[i].price);
    }

    free(result);
    return EXIT_SUCCESS;
}