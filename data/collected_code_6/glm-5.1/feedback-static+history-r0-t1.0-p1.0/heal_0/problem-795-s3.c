#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[64];
    double price;
} Item;

void swap(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Item *arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].price > arr[largest].price)
        largest = left;

    if (right < n && arr[right].price > arr[largest].price)
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void buildHeap(Item *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}

Item* find_n_cheapest(Item *dataset, int dataset_size, int n) {
    if (n <= 0 || dataset_size <= 0 || n > dataset_size) {
        return NULL;
    }

    Item *heap = (Item*)malloc(n * sizeof(Item));
    if (!heap) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        heap[i] = dataset[i];
    }

    buildHeap(heap, n);

    for (int i = n; i < dataset_size; i++) {
        if (dataset[i].price < heap[0].price) {
            heap[0] = dataset[i];
            heapify(heap, n, 0);
        }
    }

    for (int i = n - 1; i > 0; i--) {
        swap(&heap[0], &heap[i]);
        heapify(heap, i, 0);
    }

    return heap;
}

int main() {
    Item dataset[] = {
        {"Item1", 50.5},
        {"Item2", 20.0},
        {"Item3", 30.25},
        {"Item4", 10.1},
        {"Item5", 40.75},
        {"Item6", 5.99},
        {"Item7", 15.5}
    };
    int dataset_size = sizeof(dataset) / sizeof(dataset[0]);
    int n = 3;

    Item *cheapest = find_n_cheapest(dataset, dataset_size, n);
    if (!cheapest) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        printf("%s: %.2f\n", cheapest[i].name, cheapest[i].price);
    }

    free(cheapest);
    cheapest = NULL;

    return EXIT_SUCCESS;
}