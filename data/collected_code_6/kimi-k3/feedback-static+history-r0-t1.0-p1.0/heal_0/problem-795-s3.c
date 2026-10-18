#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double price;
} Item;

typedef struct {
    Item *data;
    size_t size;
    size_t capacity;
} MaxHeap;

static void swap_items(Item *a, Item *b)
{
    Item temp = *a;
    *a = *b;
    *b = temp;
}

static int heap_init(MaxHeap *heap, size_t capacity)
{
    if (heap == NULL || capacity == 0U) {
        return 0;
    }
    heap->data = (Item *)malloc(capacity * sizeof(Item));
    if (heap->data == NULL) {
        return 0;
    }
    heap->size = 0U;
    heap->capacity = capacity;
    return 1;
}

static void heap_free(MaxHeap *heap)
{
    if (heap != NULL) {
        free(heap->data);
        heap->data = NULL;
        heap->size = 0U;
        heap->capacity = 0U;
    }
}

static void heapify_up(MaxHeap *heap, size_t idx)
{
    while (idx > 0U) {
        size_t parent = (idx - 1U) / 2U;
        if (heap->data[parent].price >= heap->data[idx].price) {
            break;
        }
        swap_items(&heap->data[parent], &heap->data[idx]);
        idx = parent;
    }
}

static void heapify_down(MaxHeap *heap, size_t idx)
{
    for (;;) {
        size_t left = 2U * idx + 1U;
        size_t right = 2U * idx + 2U;
        size_t largest = idx;

        if (left < heap->size && heap->data[left].price > heap->data[largest].price) {
            largest = left;
        }
        if (right < heap->size && heap->data[right].price > heap->data[largest].price) {
            largest = right;
        }
        if (largest == idx) {
            break;
        }
        swap_items(&heap->data[idx], &heap->data[largest]);
        idx = largest;
    }
}

static int heap_push(MaxHeap *heap, Item item)
{
    if (heap == NULL || heap->size >= heap->capacity) {
        return 0;
    }
    heap->data[heap->size] = item;
    heap->size++;
    heapify_up(heap, heap->size - 1U);
    return 1;
}

static int heap_pop(MaxHeap *heap, Item *out)
{
    if (heap == NULL || heap->size == 0U) {
        return 0;
    }
    if (out != NULL) {
        *out = heap->data[0];
    }
    heap->size--;
    if (heap->size > 0U) {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0U);
    }
    return 1;
}

static const Item *heap_peek(const MaxHeap *heap)
{
    if (heap == NULL || heap->size == 0U) {
        return NULL;
    }
    return &heap->data[0];
}

size_t find_n_cheapest(const Item *items, size_t count, size_t n, Item *result)
{
    MaxHeap heap;
    size_t i;
    size_t result_count;

    if (items == NULL || result == NULL || n == 0U) {
        return 0U;
    }
    if (n > count) {
        n = count;
    }
    if (!heap_init(&heap, n)) {
        return 0U;
    }

    for (i = 0U; i < count; i++) {
        if (heap.size < n) {
            if (!heap_push(&heap, items[i])) {
                heap_free(&heap);
                return 0U;
            }
        } else {
            const Item *top = heap_peek(&heap);
            if (top != NULL && items[i].price < top->price) {
                if (!heap_pop(&heap, NULL)) {
                    heap_free(&heap);
                    return 0U;
                }
                if (!heap_push(&heap, items[i])) {
                    heap_free(&heap);
                    return 0U;
                }
            }
        }
    }

    result_count = heap.size;
    for (i = 0U; i < result_count; i++) {
        size_t pos = result_count - 1U - i;
        if (!heap_pop(&heap, &result[pos])) {
            heap_free(&heap);
            return 0U;
        }
    }

    heap_free(&heap);
    return result_count;
}

int main(void)
{
    Item items[] = {
        {1, 12.50},
        {2, 5.75},
        {3, 20.00},
        {4, 3.25},
        {5, 8.99},
        {6, 1.50},
        {7, 15.00},
        {8, 7.45}
    };
    size_t count = sizeof(items) / sizeof(items[0]);
    size_t n = 3U;
    Item *result;
    size_t found;
    size_t i;

    result = (Item *)malloc(n * sizeof(Item));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    found = find_n_cheapest(items, count, n, result);
    if (found == 0U) {
        fprintf(stderr, "No items found or an error occurred\n");
        free(result);
        return EXIT_FAILURE;
    }

    printf("The %zu cheapest items:\n", found);
    for (i = 0U; i < found; i++) {
        printf("ID: %d, Price: %.2f\n", result[i].id, result[i].price);
    }

    free(result);
    return EXIT_SUCCESS;
}