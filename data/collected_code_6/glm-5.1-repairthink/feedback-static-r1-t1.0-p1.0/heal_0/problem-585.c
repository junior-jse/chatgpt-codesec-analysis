#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[50];
    double price;
} Item;

typedef struct {
    Item *data;
    int capacity;
    int size;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (Item*)malloc(capacity * sizeof(Item));
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void destroy_heap(MinHeap* heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

void swap_items(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index].price < heap->data[parent].price) {
            swap_items(&heap->data[index], &heap->data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MinHeap* heap, int index) {
    while (1) {
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
            index = smallest;
        } else {
            break;
        }
    }
}

void push_heap(MinHeap* heap, Item item) {
    if (heap->size < heap->capacity) {
        heap->data[heap->size] = item;
        heap->size++;
        heapify_up(heap, heap->size - 1);
    } else {
        if (item.price > heap->data[0].price) {
            heap->data[0] = item;
            heapify_down(heap, 0);
        }
    }
}

Item* find_n_expensive(Item* dataset, int dataset_size, int n, int* result_size) {
    if (n <= 0 || dataset_size <= 0) {
        *result_size = 0;
        return NULL;
    }
    
    int actual_n = n > dataset_size ? dataset_size : n;
    
    MinHeap* heap = create_heap(actual_n);
    if (!heap) {
        *result_size = 0;
        return NULL;
    }
    
    for (int i = 0; i < dataset_size; i++) {
        push_heap(heap, dataset[i]);
    }
    
    Item* result = (Item*)malloc(actual_n * sizeof(Item));
    if (!result) {
        destroy_heap(heap);
        *result_size = 0;
        return NULL;
    }
    
    for (int i = actual_n - 1; i >= 0; i--) {
        result[i] = heap->data[0];
        heap->data[0] = heap->data[heap->size - 1];
        heap->size--;
        heapify_down(heap, 0);
    }
    
    destroy_heap(heap);
    *result_size = actual_n;
    return result;
}

int main() {
    Item dataset[] = {
        {"Laptop", 1200.50},
        {"Smartphone", 999.99},
        {"Tablet", 450.00},
        {"Monitor", 300.00},
        {"Keyboard", 100.00},
        {"Mouse", 50.00},
        {"Headphones", 150.00},
        {"Webcam", 80.00}
    };
    
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 3;
    int result_size = 0;
    
    Item* expensive_items = find_n_expensive(dataset, dataset_size, n, &result_size);
    
    if (expensive_items) {
        for (int i = 0; i < result_size; i++) {
            printf("%s: %.2f\n", expensive_items[i].name, expensive_items[i].price);
        }
        free(expensive_items);
    }
    
    return 0;
}