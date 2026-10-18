#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int frequency;
} Element;

typedef struct {
    Element *data;
    int capacity;
    int size;
} MinHeap;

void swap(Element *a, Element *b) {
    Element temp = *a;
    *a = *b;
    *b = temp;
}

MinHeap* create_min_heap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (heap == NULL) {
        exit(EXIT_FAILURE);
    }
    heap->data = (Element*)malloc(capacity * sizeof(Element));
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

void min_heapify(MinHeap *heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left].frequency < heap->data[smallest].frequency) {
        smallest = left;
    }

    if (right < heap->size && heap->data[right].frequency < heap->data[smallest].frequency) {
        smallest = right;
    }

    if (smallest != idx) {
        swap(&heap->data[idx], &heap->data[smallest]);
        min_heapify(heap, smallest);
    }
}

void push_min_heap(MinHeap *heap, Element item) {
    if (heap->size == heap->capacity) {
        if (item.frequency > heap->data[0].frequency) {
            heap->data[0] = item;
            min_heapify(heap, 0);
        }
        return;
    }

    int i = heap->size;
    heap->data[i] = item;
    heap->size++;

    while (i != 0 && heap->data[(i - 1) / 2].frequency > heap->data[i].frequency) {
        swap(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int* find_top_k_frequent(int **lists, int *list_sizes, int num_lists, int k, int *return_size) {
    Element *combined = NULL;
    int combined_size = 0;
    int combined_capacity = 0;

    for (int i = 0; i < num_lists; i++) {
        for (int j = 0; j < list_sizes[i]; j++) {
            if (combined_size > 0 && combined[combined_size - 1].value == lists[i][j]) {
                combined[combined_size - 1].frequency++;
            } else {
                if (combined_size >= combined_capacity) {
                    combined_capacity = combined_capacity == 0 ? 16 : combined_capacity * 2;
                    Element *temp = (Element*)realloc(combined, combined_capacity * sizeof(Element));
                    if (temp == NULL) {
                        free(combined);
                        exit(EXIT_FAILURE);
                    }
                    combined = temp;
                }
                combined[combined_size].value = lists[i][j];
                combined[combined_size].frequency = 1;
                combined_size++;
            }
        }
    }

    if (k > combined_size) {
        k = combined_size;
    }

    MinHeap *heap = create_min_heap(k);

    for (int i = 0; i < combined_size; i++) {
        push_min_heap(heap, combined[i]);
    }

    int *result = (int*)malloc(k * sizeof(int));
    if (result == NULL) {
        destroy_min_heap(heap);
        free(combined);
        exit(EXIT_FAILURE);
    }

    *return_size = k;
    for (int i = k - 1; i >= 0; i--) {
        result[i] = heap->data[0].value;
        heap->data[0] = heap->data[heap->size - 1];
        heap->size--;
        min_heapify(heap, 0);
    }

    destroy_min_heap(heap);
    free(combined);

    return result;
}

int main() {
    int list1[] = {1, 3, 5, 7};
    int list2[] = {2, 3, 6, 7, 8};
    int list3[] = {1, 3, 5, 9};
    
    int *lists[] = {list1, list2, list3};
    int list_sizes[] = {4, 5, 4};
    int num_lists = 3;
    int k = 3;
    int return_size = 0;

    int *top_k = find_top_k_frequent(lists, list_sizes, num_lists, k, &return_size);

    for (int i = 0; i < return_size; i++) {
        printf("%d ", top_k[i]);
    }
    printf("\n");

    free(top_k);

    return 0;
}