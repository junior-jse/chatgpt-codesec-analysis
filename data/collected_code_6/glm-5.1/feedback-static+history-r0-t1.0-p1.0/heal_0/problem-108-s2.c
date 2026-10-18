#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int value;
    int list_index;
    int element_index;
} HeapNode;

typedef struct {
    HeapNode *data;
    int capacity;
    int size;
} MinHeap;

typedef struct {
    int **arrays;
    int *sizes;
    int num_lists;
    int *indices;
    MinHeap heap;
} SortedIterator;

bool heap_init(MinHeap *heap, int capacity) {
    heap->data = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    if (!heap->data) return false;
    heap->capacity = capacity;
    heap->size = 0;
    return true;
}

void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(MinHeap *heap, HeapNode node) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size] = node;
    int current = heap->size;
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (heap->data[current].value < heap->data[parent].value) {
            heap_swap(&heap->data[current], &heap->data[parent]);
            current = parent;
        } else {
            break;
        }
    }
    heap->size++;
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode empty = {0, -1, -1};
    if (heap->size == 0) return empty;
    HeapNode root = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        int current = 0;
        while (true) {
            int left = 2 * current + 1;
            int right = 2 * current + 2;
            int smallest = current;
            if (left < heap->size && heap->data[left].value < heap->data[smallest].value) {
                smallest = left;
            }
            if (right < heap->size && heap->data[right].value < heap->data[smallest].value) {
                smallest = right;
            }
            if (smallest != current) {
                heap_swap(&heap->data[current], &heap->data[smallest]);
                current = smallest;
            } else {
                break;
            }
        }
    }
    return root;
}

bool iterator_init(SortedIterator *it, int **arrays, int *sizes, int num_lists) {
    if (!it || !arrays || !sizes || num_lists <= 0) return false;
    it->arrays = arrays;
    it->sizes = sizes;
    it->num_lists = num_lists;
    it->indices = (int *)calloc(num_lists, sizeof(int));
    if (!it->indices) return false;
    if (!heap_init(&it->heap, num_lists)) {
        free(it->indices);
        it->indices = NULL;
        return false;
    }
    for (int i = 0; i < num_lists; i++) {
        if (sizes[i] > 0) {
            HeapNode node;
            node.value = arrays[i][0];
            node.list_index = i;
            node.element_index = 0;
            heap_push(&it->heap, node);
        }
    }
    return true;
}

bool iterator_has_next(SortedIterator *it) {
    if (!it || !it->heap.data) return false;
    return it->heap.size > 0;
}

bool iterator_next(SortedIterator *it, int *out_value) {
    if (!it || !out_value || it->heap.size == 0) return false;
    HeapNode node = heap_pop(&it->heap);
    *out_value = node.value;
    it->indices[node.list_index] = node.element_index + 1;
    if (it->indices[node.list_index] < it->sizes[node.list_index]) {
        HeapNode next_node;
        next_node.value = it->arrays[node.list_index][it->indices[node.list_index]];
        next_node.list_index = node.list_index;
        next_node.element_index = it->indices[node.list_index];
        heap_push(&it->heap, next_node);
    }
    return true;
}

void iterator_cleanup(SortedIterator *it) {
    if (!it) return;
    if (it->indices) {
        free(it->indices);
        it->indices = NULL;
    }
    if (it->heap.data) {
        free(it->heap.data);
        it->heap.data = NULL;
    }
    it->heap.size = 0;
    it->heap.capacity = 0;
}

int main() {
    int list1[] = {1, 4, 7};
    int list2[] = {2, 5, 8, 11};
    int list3[] = {0, 3, 6, 9, 12};
    
    int num_lists = 3;
    int *arrays[] = {list1, list2, list3};
    int sizes[] = {sizeof(list1) / sizeof(list1[0]), 
                   sizeof(list2) / sizeof(list2[0]), 
                   sizeof(list3) / sizeof(list3[0])};

    SortedIterator it;
    if (!iterator_init(&it, arrays, sizes, num_lists)) {
        return EXIT_FAILURE;
    }

    int value;
    while (iterator_has_next(&it)) {
        if (iterator_next(&it, &value)) {
            printf("%d ", value);
        }
    }
    printf("\n");

    iterator_cleanup(&it);

    return EXIT_SUCCESS;
}