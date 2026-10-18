#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct {
    int value;
    int prime_index;
    int multiplier;
} HeapNode;

typedef struct {
    HeapNode *data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }
    heap->data = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void destroy_heap(MinHeap *heap) {
    if (heap != NULL) {
        free(heap->data);
        free(heap);
    }
}

void swap_nodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[parent].value <= heap->data[index].value) {
            break;
        }
        swap_nodes(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

void heapify_down(MinHeap *heap, int index) {
    while (1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        
        if (left < heap->size && heap->data[left].value < heap->data[smallest].value) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].value < heap->data[smallest].value) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        swap_nodes(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

int push(MinHeap *heap, int value, int prime_index, int multiplier) {
    if (heap->size >= heap->capacity) {
        return 0;
    }
    heap->data[heap->size].value = value;
    heap->data[heap->size].prime_index = prime_index;
    heap->data[heap->size].multiplier = multiplier;
    heapify_up(heap, heap->size);
    heap->size++;
    return 1;
}

HeapNode pop(MinHeap *heap) {
    HeapNode top = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0);
    }
    return top;
}

/* Possible weaknesses found:
 *  Parameter 'primes' can be declared as pointer to const [constParameterPointer]
 */
int nth_super_ugly_number(int n, int *primes, int k) {
    if (n <= 0 || primes == NULL || k <= 0) {
        return -1;
    }
    
    int *ugly = (int*)malloc(sizeof(int) * n);
    if (ugly == NULL) {
        return -1;
    }
    
    MinHeap *heap = create_heap(k);
    if (heap == NULL) {
        free(ugly);
        return -1;
    }
    
    ugly[0] = 1;
    
    for (int i = 0; i < k; i++) {
        if (primes[i] <= 0) {
            destroy_heap(heap);
            free(ugly);
            return -1;
        }
        if (!push(heap, primes[i], i, 0)) {
            destroy_heap(heap);
            free(ugly);
            return -1;
        }
    }
    
    int count = 1;
    int last_ugly = 1;
    
    while (count < n) {
        HeapNode node = pop(heap);
        int next_ugly = node.value;
        
        if (next_ugly != last_ugly) {
            ugly[count] = next_ugly;
            last_ugly = next_ugly;
            count++;
        }
        
        int next_multiplier = node.multiplier + 1;
        if (next_multiplier < n) {
            long long next_val = (long long)ugly[next_multiplier] * primes[node.prime_index];
            if (next_val > INT_MAX) {
                continue;
            }
            if (!push(heap, (int)next_val, node.prime_index, next_multiplier)) {
                destroy_heap(heap);
                free(ugly);
                return -1;
            }
        }
    }
    
    int result = ugly[n - 1];
    destroy_heap(heap);
    free(ugly);
    return result;
}

int main() {
    int primes[] = {2, 7, 13, 19};
    int k = sizeof(primes) / sizeof(primes[0]);
    int n = 12;
    
    int result = nth_super_ugly_number(n, primes, k);
    if (result != -1) {
        printf("%d\n", result);
    }
    
    return 0;
}