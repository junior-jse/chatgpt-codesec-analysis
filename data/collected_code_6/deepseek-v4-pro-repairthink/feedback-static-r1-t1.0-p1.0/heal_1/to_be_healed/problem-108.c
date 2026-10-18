#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int array_index;
    int element_index;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    int **arrays;
    int *lengths;
    int num_arrays;
    MinHeap *heap;
} MergeIterator;

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

/* Possible weaknesses found:
 *  Parameter 'heap' can be declared as pointer to const [constParameterPointer]
 */
int heap_is_empty(MinHeap *heap) {
    return heap->size == 0;
}

void heap_push(MinHeap *heap, int value, int array_index, int element_index) {
    if (heap->size >= heap->capacity) return;
    
    int i = heap->size;
    heap->nodes[i].value = value;
    heap->nodes[i].array_index = array_index;
    heap->nodes[i].element_index = element_index;
    heap->size++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->nodes[parent].value <= heap->nodes[i].value) break;
        heap_swap(&heap->nodes[parent], &heap->nodes[i]);
        i = parent;
    }
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        int i = 0;
        
        while (1) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            
            if (left < heap->size && heap->nodes[left].value < heap->nodes[smallest].value) {
                smallest = left;
            }
            if (right < heap->size && heap->nodes[right].value < heap->nodes[smallest].value) {
                smallest = right;
            }
            if (smallest == i) break;
            
            heap_swap(&heap->nodes[i], &heap->nodes[smallest]);
            i = smallest;
        }
    }
    
    return result;
}

MergeIterator* merge_iterator_create(int **arrays, int *lengths, int num_arrays) {
    MergeIterator *iter = (MergeIterator*)malloc(sizeof(MergeIterator));
    if (!iter) return NULL;
    
    iter->arrays = arrays;
    iter->lengths = lengths;
    iter->num_arrays = num_arrays;
    
    iter->heap = heap_create(num_arrays);
    if (!iter->heap) {
        free(iter);
        return NULL;
    }
    
    for (int i = 0; i < num_arrays; i++) {
        if (lengths[i] > 0) {
            heap_push(iter->heap, arrays[i][0], i, 0);
        }
    }
    
    return iter;
}

int merge_iterator_next(MergeIterator *iter, int *value) {
    if (!iter || heap_is_empty(iter->heap)) {
        return 0;
    }
    
    HeapNode node = heap_pop(iter->heap);
    *value = node.value;
    
    int next_element = node.element_index + 1;
    if (next_element < iter->lengths[node.array_index]) {
        heap_push(iter->heap, iter->arrays[node.array_index][next_element], 
                  node.array_index, next_element);
    }
    
    return 1;
}

void merge_iterator_destroy(MergeIterator *iter) {
    if (iter) {
        heap_destroy(iter->heap);
        free(iter);
    }
}

int main(void) {
    int arr1[] = {1, 4, 7, 10};
    int arr2[] = {2, 5, 8, 11};
    int arr3[] = {3, 6, 9, 12};
    
    int *arrays[] = {arr1, arr2, arr3};
    int lengths[] = {4, 4, 4};
    
    MergeIterator *iter = merge_iterator_create(arrays, lengths, 3);
    if (!iter) {
        fprintf(stderr, "Failed to create iterator\n");
        return 1;
    }
    
    int value;
    while (merge_iterator_next(iter, &value)) {
        printf("%d ", value);
    }
    printf("\n");
    
    merge_iterator_destroy(iter);
    
    return 0;
}