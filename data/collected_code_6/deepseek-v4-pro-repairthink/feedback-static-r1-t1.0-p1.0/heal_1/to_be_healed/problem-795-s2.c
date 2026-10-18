#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double price;
} Item;

typedef struct {
    Item *items;
    int size;
    int capacity;
} MaxHeap;

void swap(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MaxHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->items[parent].price < heap->items[index].price) {
            swap(&heap->items[parent], &heap->items[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MaxHeap *heap, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;
        
        if (left < heap->size && heap->items[left].price > heap->items[largest].price) {
            largest = left;
        }
        if (right < heap->size && heap->items[right].price > heap->items[largest].price) {
            largest = right;
        }
        
        if (largest != index) {
            swap(&heap->items[index], &heap->items[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

MaxHeap *create_heap(int capacity) {
    MaxHeap *heap = (MaxHeap *)malloc(sizeof(MaxHeap));
    if (heap == NULL) {
        return NULL;
    }
    heap->items = (Item *)malloc(capacity * sizeof(Item));
    if (heap->items == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void destroy_heap(MaxHeap *heap) {
    if (heap != NULL) {
        free(heap->items);
        free(heap);
    }
}

int heap_push(MaxHeap *heap, Item item) {
    if (heap->size >= heap->capacity) {
        return -1;
    }
    heap->items[heap->size] = item;
    heap->size++;
    heapify_up(heap, heap->size - 1);
    return 0;
}

Item heap_peek(MaxHeap *heap) {
    return heap->items[0];
}

Item heap_pop(MaxHeap *heap) {
    Item root = heap->items[0];
    heap->items[0] = heap->items[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

int heap_replace_root(MaxHeap *heap, Item item) {
    if (heap->size == 0) {
        return -1;
    }
    heap->items[0] = item;
    heapify_down(heap, 0);
    return 0;
}

Item *find_cheapest_items(Item *dataset, int dataset_size, int n, int *result_size) {
    /* Possible weaknesses found:
     *  Assuming that condition 'result_size==NULL' is not redundant
     */
    if (dataset == NULL || dataset_size <= 0 || n <= 0 || result_size == NULL) {
        /* Possible weaknesses found:
         *  Either the condition 'result_size==NULL' is redundant or there is possible null pointer dereference: result_size. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *result_size = 0;
        return NULL;
    }
    
    if (n > dataset_size) {
        n = dataset_size;
    }
    
    MaxHeap *heap = create_heap(n);
    if (heap == NULL) {
        *result_size = 0;
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        if (heap_push(heap, dataset[i]) != 0) {
            destroy_heap(heap);
            *result_size = 0;
            return NULL;
        }
    }
    
    for (int i = n; i < dataset_size; i++) {
        if (dataset[i].price < heap_peek(heap).price) {
            if (heap_replace_root(heap, dataset[i]) != 0) {
                destroy_heap(heap);
                *result_size = 0;
                return NULL;
            }
        }
    }
    
    Item *result = (Item *)malloc(n * sizeof(Item));
    if (result == NULL) {
        destroy_heap(heap);
        *result_size = 0;
        return NULL;
    }
    
    for (int i = n - 1; i >= 0; i--) {
        result[i] = heap_pop(heap);
    }
    
    destroy_heap(heap);
    *result_size = n;
    return result;
}

int main(void) {
    Item dataset[] = {
        {1, 29.99},
        {2, 89.99},
        {3, 5.49},
        {4, 49.99},
        {5, 99.99},
        {6, 15.79},
        {7, 199.99},
        {8, 9.99},
        {9, 75.00},
        {10, 2.50}
    };
    
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 4;
    int result_size = 0;
    
    Item *cheapest = find_cheapest_items(dataset, dataset_size, n, &result_size);
    
    if (cheapest == NULL) {
        printf("Error finding cheapest items\n");
        return 1;
    }
    
    printf("The %d cheapest items are:\n", result_size);
    for (int i = 0; i < result_size; i++) {
        printf("ID: %d, Price: $%.2f\n", cheapest[i].id, cheapest[i].price);
    }
    
    free(cheapest);
    return 0;
}