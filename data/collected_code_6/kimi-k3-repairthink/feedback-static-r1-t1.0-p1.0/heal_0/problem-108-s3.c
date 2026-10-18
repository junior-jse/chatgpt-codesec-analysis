#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Heap;

typedef struct {
    int *array;
    size_t size;
    size_t index;
} SortedInput;

typedef struct {
    int value;
    size_t input_index;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    size_t size;
    size_t capacity;
} MinHeap;

typedef struct {
    MinHeap *heap;
    SortedInput *inputs;
    size_t num_inputs;
    int finished;
} SortedIterator;

static int heap_init(MinHeap *heap, size_t capacity) {
    if (heap == NULL || capacity == 0) {
        return -1;
    }
    heap->nodes = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    if (heap->nodes == NULL) {
        return -1;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return 0;
}

static void heap_destroy(MinHeap *heap) {
    if (heap != NULL && heap->nodes != NULL) {
        free(heap->nodes);
        heap->nodes = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

static void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static int heap_push(MinHeap *heap, int value, size_t input_index) {
    size_t i;
    size_t parent;
    
    if (heap == NULL || heap->size >= heap->capacity) {
        return -1;
    }
    
    i = heap->size;
    heap->nodes[i].value = value;
    heap->nodes[i].input_index = input_index;
    heap->size++;
    
    while (i > 0) {
        parent = (i - 1) / 2;
        if (heap->nodes[parent].value <= heap->nodes[i].value) {
            break;
        }
        heap_swap(&heap->nodes[parent], &heap->nodes[i]);
        i = parent;
    }
    
    return 0;
}

static int heap_pop(MinHeap *heap, HeapNode *result) {
    size_t i;
    size_t left;
    size_t right;
    size_t smallest;
    
    if (heap == NULL || heap->size == 0 || result == NULL) {
        return -1;
    }
    
    *result = heap->nodes[0];
    heap->size--;
    
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        
        i = 0;
        for (;;) {
            left = 2 * i + 1;
            right = 2 * i + 2;
            smallest = i;
            
            if (left < heap->size && heap->nodes[left].value < heap->nodes[smallest].value) {
                smallest = left;
            }
            if (right < heap->size && heap->nodes[right].value < heap->nodes[smallest].value) {
                smallest = right;
            }
            
            if (smallest == i) {
                break;
            }
            
            heap_swap(&heap->nodes[i], &heap->nodes[smallest]);
            i = smallest;
        }
    }
    
    return 0;
}

SortedIterator *sorted_iterator_create(int **arrays, size_t *sizes, size_t num_inputs) {
    SortedIterator *iter;
    size_t i;
    
    if (arrays == NULL || sizes == NULL || num_inputs == 0) {
        return NULL;
    }
    
    iter = (SortedIterator *)malloc(sizeof(SortedIterator));
    if (iter == NULL) {
        return NULL;
    }
    
    iter->inputs = (SortedInput *)calloc(num_inputs, sizeof(SortedInput));
    if (iter->inputs == NULL) {
        free(iter);
        return NULL;
    }
    
    iter->heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (iter->heap == NULL) {
        free(iter->inputs);
        free(iter);
        return NULL;
    }
    
    if (heap_init(iter->heap, num_inputs) != 0) {
        free(iter->heap);
        free(iter->inputs);
        free(iter);
        return NULL;
    }
    
    iter->num_inputs = num_inputs;
    iter->finished = 0;
    
    for (i = 0; i < num_inputs; i++) {
        iter->inputs[i].array = arrays[i];
        iter->inputs[i].size = sizes[i];
        iter->inputs[i].index = 0;
        
        if (sizes[i] > 0 && arrays[i] != NULL) {
            if (heap_push(iter->heap, arrays[i][0], i) != 0) {
                heap_destroy(iter->heap);
                free(iter->heap);
                free(iter->inputs);
                free(iter);
                return NULL;
            }
            iter->inputs[i].index = 1;
        }
    }
    
    if (iter->heap->size == 0) {
        iter->finished = 1;
    }
    
    return iter;
}

int sorted_iterator_next(SortedIterator *iter, int *value) {
    HeapNode node;
    SortedInput *input;
    
    if (iter == NULL || value == NULL || iter->finished) {
        return -1;
    }
    
    if (heap_pop(iter->heap, &node) != 0) {
        iter->finished = 1;
        return -1;
    }
    
    *value = node.value;
    input = &iter->inputs[node.input_index];
    
    if (input->index < input->size) {
        if (heap_push(iter->heap, input->array[input->index], node.input_index) != 0) {
            iter->finished = 1;
            return -1;
        }
        input->index++;
    }
    
    if (iter->heap->size == 0) {
        iter->finished = 1;
    }
    
    return 0;
}

int sorted_iterator_has_next(SortedIterator *iter) {
    if (iter == NULL) {
        return 0;
    }
    return !iter->finished && iter->heap->size > 0;
}

void sorted_iterator_destroy(SortedIterator *iter) {
    if (iter != NULL) {
        if (iter->heap != NULL) {
            heap_destroy(iter->heap);
            free(iter->heap);
        }
        if (iter->inputs != NULL) {
            free(iter->inputs);
        }
        free(iter);
    }
}

int main(void) {
    int arr1[] = {1, 4, 7, 10};
    int arr2[] = {2, 5, 8, 11};
    int arr3[] = {3, 6, 9, 12};
    int arr4[] = {0, 13, 14};
    
    int *arrays[] = {arr1, arr2, arr3, arr4};
    size_t sizes[] = {4, 4, 4, 3};
    size_t num_inputs = 4;
    
    SortedIterator *iter;
    int value;
    int result;
    
    iter = sorted_iterator_create(arrays, sizes, num_inputs);
    if (iter == NULL) {
        fprintf(stderr, "Failed to create sorted iterator\n");
        return EXIT_FAILURE;
    }
    
    printf("Merged sorted output: ");
    while (sorted_iterator_has_next(iter)) {
        result = sorted_iterator_next(iter, &value);
        if (result != 0) {
            fprintf(stderr, "Error retrieving next value\n");
            sorted_iterator_destroy(iter);
            return EXIT_FAILURE;
        }
        printf("%d ", value);
    }
    printf("\n");
    
    sorted_iterator_destroy(iter);
    
    return EXIT_SUCCESS;
}