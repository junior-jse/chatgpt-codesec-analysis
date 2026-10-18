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

MaxHeap* create_heap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    if (heap == NULL) exit(EXIT_FAILURE);
    heap->data = (Item*)malloc(capacity * sizeof(Item));
    if (heap->data == NULL) {
        free(heap);
        exit(EXIT_FAILURE);
    }
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void destroy_heap(MaxHeap* heap) {
    if (heap != NULL) {
        free(heap->data);
        free(heap);
    }
}

void swap_items(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MaxHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index].price <= heap->data[parent].price) break;
        swap_items(&heap->data[index], &heap->data[parent]);
        index = parent;
    }
}

void heapify_down(MaxHeap* heap, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < heap->size && heap->data[left].price > heap->data[largest].price) largest = left;
        if (right < heap->size && heap->data[right].price > heap->data[largest].price) largest = right;

        if (largest == index) break;

        swap_items(&heap->data[index], &heap->data[largest]);
        index = largest;
    }
}

void push_heap(MaxHeap* heap, Item item) {
    if (heap->size == heap->capacity) {
        if (item.price >= heap->data[0].price) return;
        heap->data[0] = item;
        heapify_down(heap, 0);
    } else {
        heap->data[heap->size] = item;
        heap->size++;
        heapify_up(heap, heap->size - 1);
    }
}

/* Possible weaknesses found:
 *  Parameter 'dataset' can be declared as pointer to const [constParameterPointer]
 */
Item* find_n_cheapest(Item* dataset, int dataset_size, int n, int* result_size) {
    if (n <= 0 || dataset_size <= 0) {
        *result_size = 0;
        return NULL;
    }

    int actual_n = n;
    if (dataset_size < n) actual_n = dataset_size;

    MaxHeap* heap = create_heap(actual_n);

    for (int i = 0; i < dataset_size; i++) {
        push_heap(heap, dataset[i]);
    }

    Item* result = (Item*)malloc(actual_n * sizeof(Item));
    if (result == NULL) {
        destroy_heap(heap);
        *result_size = 0;
        return NULL;
    }

    *result_size = heap->size;
    int idx = heap->size - 1;
    while (heap->size > 0) {
        result[idx] = heap->data[0];
        swap_items(&heap->data[0], &heap->data[heap->size - 1]);
        heap->size--;
        heapify_down(heap, 0);
        idx--;
    }

    destroy_heap(heap);
    return result;
}

int main() {
    Item dataset[] = {
        {1, 19.99}, {2, 5.49}, {3, 12.50}, {4, 3.99},
        {5, 25.00}, {6, 8.75}, {7, 1.99}, {8, 15.30}
    };
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 4;
    int result_size = 0;

    Item* cheapest = find_n_cheapest(dataset, dataset_size, n, &result_size);
    
    if (cheapest != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("ID: %d, Price: %.2f\n", cheapest[i].id, cheapest[i].price);
        }
        free(cheapest);
    }

    return 0;
}