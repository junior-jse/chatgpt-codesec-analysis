#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double price;
} Item;

typedef struct {
    Item *data;
    int size;
    int capacity;
} MinHeap;

static void swap(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

static MinHeap *heap_create(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (Item *)malloc(sizeof(Item) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

static void heap_destroy(MinHeap *heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

static void heapify_down(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left].price < heap->data[smallest].price)
        smallest = left;
    if (right < heap->size && heap->data[right].price < heap->data[smallest].price)
        smallest = right;

    if (smallest != index) {
        swap(&heap->data[index], &heap->data[smallest]);
        heapify_down(heap, smallest);
    }
}

static void heapify_up(MinHeap *heap, int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && heap->data[index].price < heap->data[parent].price) {
        swap(&heap->data[index], &heap->data[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

static int heap_push(MinHeap *heap, Item item) {
    if (heap->size >= heap->capacity) {
        if (item.price <= heap->data[0].price)
            return 0;
        heap->data[0] = item;
        heapify_down(heap, 0);
        return 1;
    }
    heap->data[heap->size] = item;
    heap->size++;
    heapify_up(heap, heap->size - 1);
    return 1;
}

static Item heap_pop(MinHeap *heap) {
    Item top = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return top;
}

/* Possible weaknesses found:
 *  Parameter 'dataset' can be declared as pointer to const [constParameterPointer]
 */
static Item *find_expensive_items(Item *dataset, int dataset_size, int n, int *result_size) {
    /* Possible weaknesses found:
     *  Assuming that condition '!result_size' is not redundant
     */
    if (!dataset || dataset_size <= 0 || n <= 0 || !result_size) {
        /* Possible weaknesses found:
         *  Null pointer dereference
         *  Either the condition '!result_size' is redundant or there is possible null pointer dereference: result_size. [nullPointerRedundantCheck]
         */
        *result_size = 0;
        return NULL;
    }

    if (n > dataset_size)
        n = dataset_size;

    MinHeap *heap = heap_create(n);
    if (!heap) {
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < dataset_size; i++) {
        heap_push(heap, dataset[i]);
    }

    Item *result = (Item *)malloc(sizeof(Item) * n);
    if (!result) {
        heap_destroy(heap);
        *result_size = 0;
        return NULL;
    }

    *result_size = n;
    for (int i = n - 1; i >= 0; i--) {
        result[i] = heap_pop(heap);
    }

    heap_destroy(heap);
    return result;
}

int main(void) {
    Item dataset[] = {
        {1, 10.5}, {2, 20.0}, {3, 5.5}, {4, 30.0},
        {5, 15.0}, {6, 25.5}, {7, 7.5}, {8, 40.0}
    };
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 3;
    int result_size = 0;

    Item *expensive = find_expensive_items(dataset, dataset_size, n, &result_size);

    if (expensive) {
        printf("Top %d expensive items:\n", n);
        for (int i = 0; i < result_size; i++) {
            printf("ID: %d, Price: %.2f\n", expensive[i].id, expensive[i].price);
        }
        free(expensive);
    } else {
        printf("Failed to find expensive items.\n");
    }

    return 0;
}