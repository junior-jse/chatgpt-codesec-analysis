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

static void swap_items(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

static MinHeap *min_heap_create(int capacity) {
    MinHeap *heap;
    
    if (capacity <= 0) {
        return NULL;
    }
    
    heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }
    
    heap->data = (Item *)malloc((size_t)capacity * sizeof(Item));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = capacity;
    
    return heap;
}

static void min_heap_destroy(MinHeap *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->data);
    free(heap);
}

static void min_heapify_down(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < heap->size && heap->data[left].price < heap->data[smallest].price) {
        smallest = left;
    }
    
    if (right < heap->size && heap->data[right].price < heap->data[smallest].price) {
        smallest = right;
    }
    
    if (smallest != index) {
        swap_items(&heap->data[index], &heap->data[smallest]);
        min_heapify_down(heap, smallest);
    }
}

static void min_heapify_up(MinHeap *heap, int index) {
    int parent;
    
    while (index > 0) {
        parent = (index - 1) / 2;
        if (heap->data[parent].price <= heap->data[index].price) {
            break;
        }
        swap_items(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static int min_heap_push(MinHeap *heap, Item item) {
    if (heap == NULL || heap->size >= heap->capacity) {
        return -1;
    }
    
    heap->data[heap->size] = item;
    min_heapify_up(heap, heap->size);
    heap->size++;
    
    return 0;
}

static Item min_heap_pop(MinHeap *heap) {
    Item result;
    
    if (heap == NULL || heap->size == 0) {
        result.id = -1;
        result.price = -1.0;
        return result;
    }
    
    result = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    min_heapify_down(heap, 0);
    
    return result;
}

static Item min_heap_peek(const MinHeap *heap) {
    Item result;
    
    if (heap == NULL || heap->size == 0) {
        result.id = -1;
        result.price = -1.0;
        return result;
    }
    
    return heap->data[0];
}

static int compare_items_desc(const void *a, const void *b) {
    const Item *item_a = (const Item *)a;
    const Item *item_b = (const Item *)b;
    
    if (item_b->price > item_a->price) {
        return 1;
    }
    if (item_b->price < item_a->price) {
        return -1;
    }
    return 0;
}

Item *find_n_expensive_items(const Item *items, int dataset_size, int n, int *result_size) {
    MinHeap *heap;
    Item *result;
    Item min_item;
    int i;
    
    if (items == NULL || dataset_size <= 0 || n <= 0 || n > dataset_size || result_size == NULL) {
        *result_size = 0;
        return NULL;
    }
    
    heap = min_heap_create(n);
    if (heap == NULL) {
        *result_size = 0;
        return NULL;
    }
    
    for (i = 0; i < n; i++) {
        if (min_heap_push(heap, items[i]) != 0) {
            min_heap_destroy(heap);
            *result_size = 0;
            return NULL;
        }
    }
    
    for (i = n; i < dataset_size; i++) {
        min_item = min_heap_peek(heap);
        if (items[i].price > min_item.price) {
            min_heap_pop(heap);
            if (min_heap_push(heap, items[i]) != 0) {
                min_heap_destroy(heap);
                *result_size = 0;
                return NULL;
            }
        }
    }
    
    result = (Item *)malloc((size_t)n * sizeof(Item));
    if (result == NULL) {
        min_heap_destroy(heap);
        *result_size = 0;
        return NULL;
    }
    
    for (i = 0; i < n; i++) {
        result[i] = min_heap_pop(heap);
    }
    
    qsort(result, (size_t)n, sizeof(Item), compare_items_desc);
    
    *result_size = n;
    min_heap_destroy(heap);
    
    return result;
}

int main(void) {
    Item items[] = {
        {1, 10.5}, {2, 20.0}, {3, 5.25}, {4, 100.0},
        {5, 50.75}, {6, 75.0}, {7, 30.25}, {8, 90.5},
        {9, 15.0}, {10, 60.0}
    };
    int dataset_size = sizeof(items) / sizeof(items[0]);
    int n = 4;
    int result_size;
    Item *result;
    int i;
    
    result = find_n_expensive_items(items, dataset_size, n, &result_size);
    
    if (result != NULL) {
        printf("Top %d expensive items:\n", result_size);
        for (i = 0; i < result_size; i++) {
            printf("Item ID: %d, Price: %.2f\n", result[i].id, result[i].price);
        }
        free(result);
    } else {
        printf("Failed to find expensive items\n");
    }
    
    return 0;
}