#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    long long val;
    int prime_idx;
    int ugly_idx;
} HeapNode;

typedef struct {
    HeapNode *data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_nodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push_heap(MinHeap *heap, HeapNode node) {
    if (heap->size == heap->capacity) return;
    heap->data[heap->size] = node;
    int curr = heap->size;
    heap->size++;
    while (curr > 0) {
        int parent = (curr - 1) / 2;
        if (heap->data[curr].val < heap->data[parent].val) {
            swap_nodes(&heap->data[curr], &heap->data[parent]);
            curr = parent;
        } else {
            break;
        }
    }
}

HeapNode pop_heap(MinHeap *heap) {
    HeapNode popped = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    int curr = 0;
    while (1) {
        int left = 2 * curr + 1;
        int right = 2 * curr + 2;
        int smallest = curr;
        if (left < heap->size && heap->data[left].val < heap->data[smallest].val) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].val < heap->data[smallest].val) {
            smallest = right;
        }
        if (smallest != curr) {
            swap_nodes(&heap->data[curr], &heap->data[smallest]);
            curr = smallest;
        } else {
            break;
        }
    }
    return popped;
}

int nthSuperUglyNumber(int n, int *primes, int k) {
    if (n <= 0 || k <= 0) return 0;
    if (n == 1) return 1;
    
    long long *ugly = (long long*)malloc(sizeof(long long) * n);
    int *indices = (int*)calloc(k, sizeof(int));
    ugly[0] = 1;
    
    MinHeap *heap = create_heap(k + n * 2);
    for (int i = 0; i < k; i++) {
        HeapNode node;
        node.val = primes[i];
        node.prime_idx = i;
        node.ugly_idx = 0;
        push_heap(heap, node);
    }
    
    int count = 1;
    while (count < n) {
        HeapNode min_node = pop_heap(heap);
        if (min_node.val != ugly[count - 1]) {
            ugly[count] = min_node.val;
            count++;
        }
        
        int p_idx = min_node.prime_idx;
        int u_idx = min_node.ugly_idx;
        
        if (ugly[u_idx] <= LLONG_MAX / primes[p_idx] && indices[p_idx] == u_idx) {
            indices[p_idx]++;
            HeapNode next_node;
            next_node.val = ugly[u_idx + 1] * primes[p_idx];
            next_node.prime_idx = p_idx;
            next_node.ugly_idx = u_idx + 1;
            push_heap(heap, next_node);
        }
    }
    
    int result = (int)ugly[n - 1];
    
    free(ugly);
    free(indices);
    free(heap->data);
    free(heap);
    
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