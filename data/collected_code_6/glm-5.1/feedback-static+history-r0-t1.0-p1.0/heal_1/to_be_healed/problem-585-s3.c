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
} MinHeap;

MinHeap* create_min_heap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (heap == NULL) exit(EXIT_FAILURE);
    heap->data = (Item *)malloc(capacity * sizeof(Item));
    if (heap->data == NULL) {
        free(heap);
        exit(EXIT_FAILURE);
    }
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void destroy_min_heap(MinHeap *heap) {
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

void min_heapify_up(MinHeap *heap, int index) {
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

void min_heapify_down(MinHeap *heap, int index) {
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

void push_min_heap(MinHeap *heap, Item item) {
    if (heap->size == heap->capacity) {
        if (item.price > heap->data[0].price) {
            heap->data[0] = item;
            min_heapify_down(heap, 0);
        }
    } else {
        heap->data[heap->size] = item;
        min_heapify_up(heap, heap->size);
        heap->size++;
    }
}

/* Possible weaknesses found:
 *  Parameter 'dataset' can be declared as pointer to const [constParameterPointer]
 */
Item* find_n_expensive(Item *dataset, int dataset_size, int n, int *result_size) {
    if (n <= 0) {
        *result_size = 0;
        return NULL;
    }
    
    int valid_n = n;
    if (dataset_size < n) {
        valid_n = dataset_size;
    }

    MinHeap *heap = create_min_heap(valid_n);
    if (heap == NULL) {
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < dataset_size; i++) {
        push_min_heap(heap, dataset[i]);
    }

    Item *result = (Item *)malloc(valid_n * sizeof(Item));
    if (result == NULL) {
        destroy_min_heap(heap);
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < valid_n; i++) {
        result[i] = heap->data[0];
        heap->data[0] = heap->data[heap->size - 1];
        heap->size--;
        min_heapify_down(heap, 0);
    }

    for (int i = 0; i < valid_n / 2; i++) {
        swap_items(&result[i], &result[valid_n - 1 - i]);
    }

    *result_size = valid_n;
    destroy_min_heap(heap);
    return result;
}

int main() {
    Item dataset[] = {
        {1, 10.5},
        {2, 150.0},
        {3, 45.2},
        {4, 89.9},
        {5, 210.5},
        {6, 15.0},
        {7, 500.0},
        {8, 75.3}
    };
    
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 4;
    int result_size = 0;
    
    Item *expensive_items = find_n_expensive(dataset, dataset_size, n, &result_size);
    
    if (expensive_items != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("ID: %d, Price: %.2f\n", expensive_items[i].id, expensive_items[i].price);
        }
        free(expensive_items);
    }
    
    return 0;
}