#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long value;
    int prime_index;
    long long last_value;
} HeapNode;

typedef struct {
    HeapNode *data;
    int capacity;
    int size;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void swap_nodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push_heap(MinHeap* heap, HeapNode node) {
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

HeapNode pop_heap(MinHeap* heap) {
    HeapNode empty = {0, 0, 0};
    if (heap->size == 0) return empty;
    HeapNode root = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
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
    }
    return root;
}

int nth_super_ugly_number(int n, int* primes, int k) {
    if (n <= 0 || k <= 0 || primes == NULL) return 0;
    
    long long *ugly = (long long*)malloc(n * sizeof(long long));
    ugly[0] = 1;
    
    MinHeap* heap = create_heap(n * k);
    for (int i = 0; i < k; i++) {
        HeapNode node;
        node.value = (long long)primes[i];
        node.prime_index = i;
        node.last_value = 1;
        push_heap(heap, node);
    }
    
    for (int i = 1; i < n; i++) {
        HeapNode top = pop_heap(heap);
        ugly[i] = top.value;
        
        while (heap->size > 0 && heap->data[0].value == top.value) {
            HeapNode dup = pop_heap(heap);
            HeapNode next;
            next.prime_index = dup.prime_index;
            next.last_value = ugly[dup.prime_index + 1];
            next.value = next.last_value * (long long)primes[next.prime_index];
            push_heap(heap, next);
        }
        
        HeapNode next;
        next.prime_index = top.prime_index;
        next.last_value = ugly[top.prime_index + 1];
        next.value = next.last_value * (long long)primes[next.prime_index];
        push_heap(heap, next);
    }
    
    long long result = ugly[n - 1];
    free(ugly);
    free(heap->data);
    free(heap);
    return (int)result;
}

int main() {
    int primes[] = {2, 7, 13, 19};
    int k = sizeof(primes) / sizeof(primes[0]);
    int n = 12;
    int result = nth_super_ugly_number(n, primes, k);
    printf("%d\n", result);
    return 0;
}