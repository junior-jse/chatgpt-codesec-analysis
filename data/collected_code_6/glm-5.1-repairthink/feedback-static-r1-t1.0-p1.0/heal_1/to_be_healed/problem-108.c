#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int value;
    int list_index;
} HeapNode;

typedef struct {
    HeapNode *data;
    int capacity;
    int size;
} MinHeap;

typedef struct {
    int *elements;
    int length;
    int current_index;
} SortedInput;

typedef struct {
    MinHeap *heap;
    SortedInput *inputs;
    int num_inputs;
} SortedIterator;

MinHeap *create_heap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void destroy_heap(MinHeap *heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

void swap_nodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(MinHeap *heap, HeapNode node) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size] = node;
    int current = heap->size;
    heap->size++;
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (heap->data[current].value < heap->data[parent].value) {
            swap_nodes(&heap->data[current], &heap->data[parent]);
            current = parent;
        } else {
            break;
        }
    }
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode empty = {0, -1};
    if (heap->size <= 0) return empty;
    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    int current = 0;
    while (1) {
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
            swap_nodes(&heap->data[current], &heap->data[smallest]);
            current = smallest;
        } else {
            break;
        }
    }
    return root;
}

/* Possible weaknesses found:
 *  Parameter 'heap' can be declared as pointer to const [constParameterPointer]
 */
bool heap_is_empty(MinHeap *heap) {
    return heap->size == 0;
}

/* Possible weaknesses found:
 *  Parameter 'inputs' can be declared as pointer to const [constParameterPointer]
 */
SortedIterator *create_iterator(SortedInput *inputs, int num_inputs) {
    SortedIterator *it = (SortedIterator *)malloc(sizeof(SortedIterator));
    if (!it) return NULL;
    it->heap = create_heap(num_inputs);
    if (!it->heap) {
        free(it);
        return NULL;
    }
    it->inputs = (SortedInput *)malloc(num_inputs * sizeof(SortedInput));
    if (!it->inputs) {
        destroy_heap(it->heap);
        free(it);
        return NULL;
    }
    it->num_inputs = num_inputs;
    for (int i = 0; i < num_inputs; i++) {
        it->inputs[i] = inputs[i];
        if (it->inputs[i].length > 0) {
            HeapNode node;
            node.value = it->inputs[i].elements[it->inputs[i].current_index];
            node.list_index = i;
            heap_push(it->heap, node);
            it->inputs[i].current_index++;
        }
    }
    return it;
}

void destroy_iterator(SortedIterator *it) {
    if (it) {
        destroy_heap(it->heap);
        free(it->inputs);
        free(it);
    }
}

bool iterator_has_next(SortedIterator *it) {
    if (!it || !it->heap) return false;
    return !heap_is_empty(it->heap);
}

int iterator_next(SortedIterator *it) {
    if (!it || !iterator_has_next(it)) {
        fprintf(stderr, "No more elements\n");
        exit(EXIT_FAILURE);
    }
    HeapNode top = heap_pop(it->heap);
    int list_idx = top.list_index;
    if (it->inputs[list_idx].current_index < it->inputs[list_idx].length) {
        HeapNode next_node;
        next_node.value = it->inputs[list_idx].elements[it->inputs[list_idx].current_index];
        next_node.list_index = list_idx;
        heap_push(it->heap, next_node);
        it->inputs[list_idx].current_index++;
    }
    return top.value;
}

int main() {
    int list1[] = {1, 4, 7, 10};
    int list2[] = {2, 5, 8, 11};
    int list3[] = {3, 6, 9, 12};
    int len1 = sizeof(list1) / sizeof(list1[0]);
    int len2 = sizeof(list2) / sizeof(list2[0]);
    int len3 = sizeof(list3) / sizeof(list3[0]);
    
    SortedInput inputs[3];
    inputs[0].elements = list1;
    inputs[0].length = len1;
    inputs[0].current_index = 0;
    
    inputs[1].elements = list2;
    inputs[1].length = len2;
    inputs[1].current_index = 0;
    
    inputs[2].elements = list3;
    inputs[2].length = len3;
    inputs[2].current_index = 0;
    
    SortedIterator *it = create_iterator(inputs, 3);
    if (!it) {
        fprintf(stderr, "Failed to create iterator\n");
        return EXIT_FAILURE;
    }
    
    while (iterator_has_next(it)) {
        printf("%d ", iterator_next(it));
    }
    printf("\n");
    
    destroy_iterator(it);
    
    return EXIT_SUCCESS;
}