#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int value;
    int source_idx;
    size_t element_idx;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    size_t size;
    size_t capacity;
} MinHeap;

typedef struct {
    int **arrays;
    size_t *lengths;
    size_t num_arrays;
    MinHeap heap;
} SortedIterator;

static void swap_nodes(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static bool heap_init(MinHeap *heap, size_t capacity)
{
    if (heap == NULL || capacity == 0) {
        return false;
    }
    
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (heap->nodes == NULL) {
        return false;
    }
    
    heap->size = 0;
    heap->capacity = capacity;
    return true;
}

static void heap_destroy(MinHeap *heap)
{
    if (heap != NULL && heap->nodes != NULL) {
        free(heap->nodes);
        heap->nodes = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static bool heap_push(MinHeap *heap, HeapNode node)
{
    size_t i;
    
    if (heap == NULL || heap->size >= heap->capacity) {
        return false;
    }
    
    i = heap->size++;
    heap->nodes[i] = node;
    
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (heap->nodes[parent].value <= heap->nodes[i].value) {
            break;
        }
        swap_nodes(&heap->nodes[parent], &heap->nodes[i]);
        i = parent;
    }
    
    return true;
}

static bool heap_pop(MinHeap *heap, HeapNode *result)
{
    if (heap == NULL || result == NULL || heap->size == 0) {
        return false;
    }
    
    *result = heap->nodes[0];
    heap->size--;
    
    if (heap->size > 0) {
        size_t i = 0;
        heap->nodes[0] = heap->nodes[heap->size];
        
        for (;;) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;
            size_t smallest = i;
            
            if (left < heap->size && 
                heap->nodes[left].value < heap->nodes[smallest].value) {
                smallest = left;
            }
            
            if (right < heap->size && 
                heap->nodes[right].value < heap->nodes[smallest].value) {
                smallest = right;
            }
            
            if (smallest == i) {
                break;
            }
            
            swap_nodes(&heap->nodes[i], &heap->nodes[smallest]);
            i = smallest;
        }
    }
    
    return true;
}

static bool iterator_init(SortedIterator *iter, int **arrays, 
                         size_t *lengths, size_t num_arrays)
{
    size_t i;
    
    if (iter == NULL || arrays == NULL || lengths == NULL || num_arrays == 0) {
        return false;
    }
    
    iter->arrays = arrays;
    iter->lengths = lengths;
    iter->num_arrays = num_arrays;
    
    if (!heap_init(&iter->heap, num_arrays)) {
        return false;
    }
    
    for (i = 0; i < num_arrays; i++) {
        if (lengths[i] > 0 && arrays[i] != NULL) {
            HeapNode node = {arrays[i][0], (int)i, 0};
            if (!heap_push(&iter->heap, node)) {
                heap_destroy(&iter->heap);
                return false;
            }
        }
    }
    
    return true;
}

static void iterator_destroy(SortedIterator *iter)
{
    if (iter != NULL) {
        heap_destroy(&iter->heap);
        iter->arrays = NULL;
        iter->lengths = NULL;
        iter->num_arrays = 0;
    }
}

static bool iterator_next(SortedIterator *iter, int *value)
{
    HeapNode node;
    
    if (iter == NULL || value == NULL) {
        return false;
    }
    
    if (!heap_pop(&iter->heap, &node)) {
        return false;
    }
    
    *value = node.value;
    node.element_idx++;
    
    if (node.element_idx < iter->lengths[node.source_idx]) {
        node.value = iter->arrays[node.source_idx][node.element_idx];
        if (!heap_push(&iter->heap, node)) {
            return false;
        }
    }
    
    return true;
}

int main(void)
{
    int arr1[] = {1, 4, 7, 10};
    int arr2[] = {2, 5, 8, 11};
    int arr3[] = {3, 6, 9, 12};
    int arr4[] = {0, 13, 14};
    
    int *arrays[] = {arr1, arr2, arr3, arr4};
    size_t lengths[] = {4, 4, 4, 3};
    size_t num_arrays = 4;
    
    SortedIterator iter;
    int value;
    
    if (!iterator_init(&iter, arrays, lengths, num_arrays)) {
        fprintf(stderr, "Failed to initialize iterator\n");
        return EXIT_FAILURE;
    }
    
    printf("Merged sorted output: ");
    while (iterator_next(&iter, &value)) {
        printf("%d ", value);
    }
    printf("\n");
    
    iterator_destroy(&iter);
    
    return EXIT_SUCCESS;
}