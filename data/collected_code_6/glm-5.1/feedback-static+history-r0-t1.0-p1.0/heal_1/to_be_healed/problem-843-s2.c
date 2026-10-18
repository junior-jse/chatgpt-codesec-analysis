#include <stdio.h>
#include <stdlib.h>
 /* Possible weaknesses found:
  *  'LLONG_MAX' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
  */

typedef struct {
    long long value;
    int prime_index;
} HeapNode;

typedef struct {
    HeapNode *data;
    int capacity;
    int size;
} MinHeap;

void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

MinHeap* create_heap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void destroy_heap(MinHeap *heap) {
    free(heap->data);
    free(heap);
}

void push_heap(MinHeap *heap, long long value, int prime_index) {
    if (heap->size == heap->capacity) return;
    int i = heap->size++;
    heap->data[i].value = value;
    heap->data[i].prime_index = prime_index;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->data[parent].value > heap->data[i].value) {
            swap(&heap->data[parent], &heap->data[i]);
            i = parent;
        } else {
            break;
        }
    }
}

HeapNode pop_heap(MinHeap *heap) {
    if (heap->size == 0) {
        HeapNode null_node = {0, -1};
        return null_node;
    }
    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap->size && heap->data[left].value < heap->data[smallest].value) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].value < heap->data[smallest].value) {
            smallest = right;
        }
        if (smallest != i) {
            swap(&heap->data[i], &heap->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return root;
}

/* Possible weaknesses found:
 *  Parameter 'primes' can be declared as pointer to const [constParameterPointer]
 */
int nthSuperUglyNumber(int n, int* primes, int primesSize) {
    if (n <= 0 || primes == NULL || primesSize <= 0) {
        return 0;
    }
    MinHeap *heap = create_heap(primesSize * n);
    long long *ugly = (long long*)malloc(n * sizeof(long long));
    int *indices = (int*)calloc(primesSize, sizeof(int));
    
    ugly[0] = 1;
    
    for (int i = 0; i < primesSize; i++) {
        push_heap(heap, (long long)primes[i], i);
    }
    
    for (int i = 1; i < n; i++) {
        HeapNode top = pop_heap(heap);
        ugly[i] = top.value;
        int p_idx = top.prime_index;
        
        while (heap->size > 0 && heap->data[0].value == top.value) {
            HeapNode dup = pop_heap(heap);
            indices[dup.prime_index]++;
            /* Possible weaknesses found:
             *  each undeclared identifier is reported only once for each function it appears in
             *  'LLONG_MAX' undeclared (first use in this function)
             *  use of undeclared identifier 'LLONG_MAX'
             */
            if (ugly[indices[dup.prime_index]] <= LLONG_MAX / primes[dup.prime_index]) {
                long long next_val = ugly[indices[dup.prime_index]] * primes[dup.prime_index];
                push_heap(heap, next_val, dup.prime_index);
            }
        }
        
        indices[p_idx]++;
        /* Possible weaknesses found:
         *  use of undeclared identifier 'LLONG_MAX'
         */
        if (ugly[indices[p_idx]] <= LLONG_MAX / primes[p_idx]) {
            long long next_val = ugly[indices[p_idx]] * primes[p_idx];
            push_heap(heap, next_val, p_idx);
        }
    }
    
    int result = (int)ugly[n - 1];
    free(ugly);
    free(indices);
    destroy_heap(heap);
    return result;
}

int main() {
    int primes[] = {2, 7, 13, 19};
    int k = sizeof(primes) / sizeof(primes[0]);
    int n = 12;
    int result = nthSuperUglyNumber(n, primes, k);
    printf("%d\n", result);
    return 0;
}