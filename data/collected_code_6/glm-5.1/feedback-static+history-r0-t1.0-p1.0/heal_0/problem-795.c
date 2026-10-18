#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double price;
} Item;

typedef struct {
    Item *data;
    int capacity;
    int size;
} MaxHeap;

void swap(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

MaxHeap* create_heap(int capacity) {
    MaxHeap *heap = (MaxHeap*)malloc(sizeof(MaxHeap));
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

void destroy_heap(MaxHeap *heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

void heapify_up(MaxHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index].price > heap->data[parent].price) {
            swap(&heap->data[index], &heap->data[parent]);
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
            swap(&heap->data[index], &heap->data[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

void push_heap(MaxHeap *heap, Item item) {
    if (heap->size == heap->capacity) {
        if (item.price < heap->data[0].price) {
            heap->data[0] = item;
            heapify_down(heap, 0);
        }
    } else {
        heap->data[heap->size] = item;
        heapify_up(heap, heap->size);
        heap->size++;
    }
}

Item* find_n_cheapest(Item *dataset, int dataset_size, int n, int *result_size) {
    if (n <= 0) {
        *result_size = 0;
        return NULL;
    }
    
    int k = (n < dataset_size) ? n : dataset_size;
    MaxHeap *heap = create_heap(k);
    if (!heap) {
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < dataset_size; i++) {
        push_heap(heap, dataset[i]);
    }

    Item *result = (Item*)malloc(k * sizeof(Item));
    if (!result) {
        destroy_heap(heap);
        *result_size = 0;
        return NULL;
    }

    for (int i = k - 1; i >= 0; i--) {
        result[i] = heap->data[0];
        heap->data[0] = heap->data[heap->size - 1];
        heap->size--;
        heapify_down(heap, 0);
    }

    destroy_heap(heap);
    *result_size = k;
    return result;
}

int main() {
    Item dataset[] = {
        {1, 10.5},
        {2, 5.2},
        {3, 20.0},
        {4, 1.5},
        {5, 8.9},
        {6, 15.0},
        {7, 3.3}
    };
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 3;
    int result_size = 0;

    Item *cheapest = find_n_cheapest(dataset, dataset_size, n, &result_size);
    
    if (cheapest) {
        for (int i = 0; i < result_size; i++) {
            printf("Item ID: %d, Price: %.2f\n", cheapest[i].id, cheapest[i].price);
        }
        free(cheapest);
    }

    return 0;
}