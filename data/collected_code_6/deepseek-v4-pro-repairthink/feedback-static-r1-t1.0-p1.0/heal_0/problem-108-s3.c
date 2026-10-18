#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    int length;
    int index;
} SortedArray;

typedef struct {
    int value;
    int array_index;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int size;
    int capacity;
} MinHeap;

MinHeap* heap_create(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->nodes = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    if (!heap->nodes) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_destroy(MinHeap *heap) {
    if (heap) {
        free(heap->nodes);
        free(heap);
    }
}

void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_down(MinHeap *heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].value < heap->nodes[smallest].value)
        smallest = left;
    if (right < heap->size && heap->nodes[right].value < heap->nodes[smallest].value)
        smallest = right;

    if (smallest != idx) {
        heap_swap(&heap->nodes[idx], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

void heapify_up(MinHeap *heap, int idx) {
    int parent = (idx - 1) / 2;
    while (idx > 0 && heap->nodes[idx].value < heap->nodes[parent].value) {
        heap_swap(&heap->nodes[idx], &heap->nodes[parent]);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

int heap_push(MinHeap *heap, int value, int array_index) {
    if (heap->size >= heap->capacity) return -1;

    heap->nodes[heap->size].value = value;
    heap->nodes[heap->size].array_index = array_index;
    heap->size++;
    heapify_up(heap, heap->size - 1);
    return 0;
}

int heap_pop(MinHeap *heap, HeapNode *result) {
    if (heap->size == 0) return -1;

    *result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    if (heap->size > 0)
        heapify_down(heap, 0);
    return 0;
}

typedef struct {
    SortedArray *arrays;
    int num_arrays;
    MinHeap *heap;
} MergeIterator;

MergeIterator* merge_iterator_create(SortedArray *arrays, int num_arrays) {
    if (!arrays || num_arrays <= 0) return NULL;

    MergeIterator *iter = (MergeIterator*)malloc(sizeof(MergeIterator));
    if (!iter) return NULL;

    iter->arrays = arrays;
    iter->num_arrays = num_arrays;
    iter->heap = heap_create(num_arrays);
    if (!iter->heap) {
        free(iter);
        return NULL;
    }

    for (int i = 0; i < num_arrays; i++) {
        if (arrays[i].length > 0) {
            if (heap_push(iter->heap, arrays[i].values[0], i) != 0) {
                heap_destroy(iter->heap);
                free(iter);
                return NULL;
            }
            arrays[i].index = 1;
        } else {
            arrays[i].index = 0;
        }
    }

    return iter;
}

void merge_iterator_destroy(MergeIterator *iter) {
    if (iter) {
        heap_destroy(iter->heap);
        free(iter);
    }
}

int merge_iterator_next(MergeIterator *iter, int *value) {
    if (!iter || !value) return 0;

    HeapNode node;
    if (heap_pop(iter->heap, &node) != 0) return 0;

    *value = node.value;

    int arr_idx = node.array_index;
    SortedArray *arr = &iter->arrays[arr_idx];

    if (arr->index < arr->length) {
        if (heap_push(iter->heap, arr->values[arr->index], arr_idx) != 0) {
            return -1;
        }
        arr->index++;
    }

    return 1;
}

int merge_iterator_has_next(MergeIterator *iter) {
    return iter && iter->heap->size > 0;
}

int main(void) {
    int arr1_data[] = {1, 3, 5, 7};
    int arr2_data[] = {2, 4, 6, 8};
    int arr3_data[] = {0, 9, 10};

    SortedArray arrays[3];
    arrays[0].values = arr1_data;
    arrays[0].length = 4;
    arrays[1].values = arr2_data;
    arrays[1].length = 4;
    arrays[2].values = arr3_data;
    arrays[2].length = 3;

    MergeIterator *iter = merge_iterator_create(arrays, 3);
    if (!iter) {
        printf("Failed to create iterator\n");
        return 1;
    }

    int value;
    while (merge_iterator_has_next(iter)) {
        int status = merge_iterator_next(iter, &value);
        if (status == 1) {
            printf("%d ", value);
        } else {
            break;
        }
    }
    printf("\n");

    merge_iterator_destroy(iter);
    return 0;
}