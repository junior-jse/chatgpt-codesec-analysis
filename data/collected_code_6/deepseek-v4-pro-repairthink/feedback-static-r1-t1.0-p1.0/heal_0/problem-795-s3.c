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
} MaxHeap;

MaxHeap* heap_create(int capacity) {
    MaxHeap *heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    if (!heap) return NULL;
    heap->data = (Item*)malloc(sizeof(Item) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_destroy(MaxHeap *heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

void heap_swap(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MaxHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[parent].price < heap->data[index].price) {
            heap_swap(&heap->data[parent], &heap->data[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MaxHeap *heap, int index) {
    while (1) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < heap->size && heap->data[left].price > heap->data[largest].price) {
            largest = left;
        }
        if (right < heap->size && heap->data[right].price > heap->data[largest].price) {
            largest = right;
        }
        if (largest != index) {
            heap_swap(&heap->data[index], &heap->data[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

int heap_push(MaxHeap *heap, Item item) {
    if (heap->size >= heap->capacity) return -1;
    heap->data[heap->size] = item;
    heap->size++;
    heapify_up(heap, heap->size - 1);
    return 0;
}

int heap_pop(MaxHeap *heap, Item *item) {
    if (heap->size <= 0) return -1;
    *item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return 0;
}

Item heap_peek(MaxHeap *heap) {
    return heap->data[0];
}

Item* find_n_cheapest(Item *items, int total_items, int n, int *result_count) {
    if (!items || total_items <= 0 || n <= 0 || n > total_items || !result_count) {
        *result_count = 0;
        return NULL;
    }
    
    MaxHeap *heap = heap_create(n);
    if (!heap) {
        *result_count = 0;
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        heap_push(heap, items[i]);
    }
    
    for (int i = n; i < total_items; i++) {
        if (items[i].price < heap_peek(heap).price) {
            Item discarded;
            heap_pop(heap, &discarded);
            heap_push(heap, items[i]);
        }
    }
    
    Item *result = (Item*)malloc(sizeof(Item) * n);
    if (!result) {
        heap_destroy(heap);
        *result_count = 0;
        return NULL;
    }
    
    for (int i = n - 1; i >= 0; i--) {
        heap_pop(heap, &result[i]);
    }
    
    *result_count = n;
    heap_destroy(heap);
    return result;
}

int main(void) {
    Item dataset[] = {
        {1, 19.99},
        {2, 5.49},
        {3, 12.99},
        {4, 8.75},
        {5, 25.00},
        {6, 3.50},
        {7, 15.25},
        {8, 7.80}
    };
    
    int total_items = sizeof(dataset) / sizeof(dataset[0]);
    int n = 3;
    int result_count = 0;
    
    Item *cheapest = find_n_cheapest(dataset, total_items, n, &result_count);
    
    if (cheapest && result_count > 0) {
        printf("The %d cheapest items are:\n", result_count);
        for (int i = 0; i < result_count; i++) {
            printf("ID: %d, Price: $%.2f\n", cheapest[i].id, cheapest[i].price);
        }
        free(cheapest);
    } else {
        printf("Could not find cheapest items.\n");
    }
    
    return 0;
}