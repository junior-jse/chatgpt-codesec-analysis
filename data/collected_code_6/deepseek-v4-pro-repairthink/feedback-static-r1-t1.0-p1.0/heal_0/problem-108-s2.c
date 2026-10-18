#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Iterator {
    int *data;
    int length;
    int index;
} Iterator;

typedef struct HeapNode {
    int value;
    int iterator_index;
} HeapNode;

typedef struct Heap {
    HeapNode *nodes;
    int size;
    int capacity;
} Heap;

void heap_init(Heap *heap, int capacity) {
    heap->nodes = (HeapNode *)malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
}

void heap_destroy(Heap *heap) {
    free(heap->nodes);
    heap->nodes = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(Heap *heap, int value, int iterator_index) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = (HeapNode *)realloc(heap->nodes, sizeof(HeapNode) * heap->capacity);
    }
    
    int i = heap->size;
    heap->nodes[i].value = value;
    heap->nodes[i].iterator_index = iterator_index;
    heap->size++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->nodes[parent].value <= heap->nodes[i].value) {
            break;
        }
        heap_swap(&heap->nodes[parent], &heap->nodes[i]);
        i = parent;
    }
}

HeapNode heap_pop(Heap *heap) {
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
            if (smallest == i) {
                break;
            }
            
            heap_swap(&heap->nodes[i], &heap->nodes[smallest]);
            i = smallest;
        }
    }
    
    return result;
}

typedef struct MergeIterator {
    Iterator **iterators;
    int iterator_count;
    Heap heap;
    int *current_values;
    int *active;
} MergeIterator;

MergeIterator *merge_iterator_create(Iterator **iterators, int count) {
    MergeIterator *mi = (MergeIterator *)malloc(sizeof(MergeIterator));
    if (!mi) {
        return NULL;
    }
    
    mi->iterators = iterators;
    mi->iterator_count = count;
    mi->active = (int *)calloc(count, sizeof(int));
    mi->current_values = (int *)malloc(sizeof(int) * count);
    
    if (!mi->active || !mi->current_values) {
        free(mi->active);
        free(mi->current_values);
        free(mi);
        return NULL;
    }
    
    heap_init(&mi->heap, count > 0 ? count : 1);
    
    for (int i = 0; i < count; i++) {
        if (iterators[i]->index < iterators[i]->length) {
            mi->active[i] = 1;
            mi->current_values[i] = iterators[i]->data[iterators[i]->index];
            heap_push(&mi->heap, mi->current_values[i], i);
        } else {
            mi->active[i] = 0;
        }
    }
    
    return mi;
}

int merge_iterator_next(MergeIterator *mi, int *value) {
    if (mi->heap.size == 0) {
        return 0;
    }
    
    HeapNode node = heap_pop(&mi->heap);
    *value = node.value;
    int idx = node.iterator_index;
    
    mi->iterators[idx]->index++;
    
    if (mi->iterators[idx]->index < mi->iterators[idx]->length) {
        mi->current_values[idx] = mi->iterators[idx]->data[mi->iterators[idx]->index];
        heap_push(&mi->heap, mi->current_values[idx], idx);
    } else {
        mi->active[idx] = 0;
    }
    
    return 1;
}

int merge_iterator_has_next(MergeIterator *mi) {
    return mi->heap.size > 0;
}

void merge_iterator_destroy(MergeIterator *mi) {
    if (!mi) {
        return;
    }
    heap_destroy(&mi->heap);
    free(mi->active);
    free(mi->current_values);
    free(mi);
}

int main(void) {
    int data1[] = {1, 3, 5, 7, 9};
    int data2[] = {2, 4, 6, 8, 10};
    int data3[] = {0, 11, 12, 13};
    
    Iterator iter1 = {data1, 5, 0};
    Iterator iter2 = {data2, 5, 0};
    Iterator iter3 = {data3, 4, 0};
    
    Iterator *iterators[] = {&iter1, &iter2, &iter3};
    
    MergeIterator *mi = merge_iterator_create(iterators, 3);
    if (!mi) {
        fprintf(stderr, "Failed to create merge iterator\n");
        return 1;
    }
    
    int value;
    while (merge_iterator_has_next(mi)) {
        if (merge_iterator_next(mi, &value)) {
            printf("%d ", value);
        }
    }
    printf("\n");
    
    merge_iterator_destroy(mi);
    
    return 0;
}