#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long val;
    int prime;
    int index;
} HeapNode;

typedef struct {
    HeapNode *data;
    int size;
    int capacity;
} MinHeap;

MinHeap *createHeap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (HeapNode *)malloc(sizeof(HeapNode) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapNodes(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapifyDown(MinHeap *heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && heap->data[left].val < heap->data[smallest].val)
        smallest = left;
    if (right < heap->size && heap->data[right].val < heap->data[smallest].val)
        smallest = right;
    
    if (smallest != idx) {
        swapNodes(&heap->data[idx], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

void heapifyUp(MinHeap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent].val > heap->data[idx].val) {
            swapNodes(&heap->data[parent], &heap->data[idx]);
            idx = parent;
        } else {
            break;
        }
    }
}

void heapPush(MinHeap *heap, HeapNode node) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode heapPop(MinHeap *heap) {
    HeapNode top = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return top;
}

int nthSuperUglyNumber(int n, int *primes, int primesSize) {
    if (n <= 0 || primesSize <= 0) return 0;
    
    long *ugly = (long *)malloc(sizeof(long) * n);
    if (!ugly) return 0;
    
    MinHeap *heap = createHeap(primesSize);
    if (!heap) {
        free(ugly);
        return 0;
    }
    
    ugly[0] = 1;
    
    for (int i = 0; i < primesSize; i++) {
        HeapNode node;
        node.val = (long)primes[i];
        node.prime = primes[i];
        node.index = 0;
        heapPush(heap, node);
    }
    
    for (int i = 1; i < n; i++) {
        ugly[i] = heap->data[0].val;
        
        while (heap->size > 0 && heap->data[0].val == ugly[i]) {
            HeapNode node = heapPop(heap);
            node.index++;
            node.val = (long)node.prime * ugly[node.index];
            heapPush(heap, node);
        }
    }
    
    int result = (int)ugly[n - 1];
    free(ugly);
    free(heap->data);
    free(heap);
    return result;
}

int main(void) {
    int primes1[] = {2, 7, 13, 19};
    int n1 = 12;
    int size1 = sizeof(primes1) / sizeof(primes1[0]);
    printf("%d\n", nthSuperUglyNumber(n1, primes1, size1));
    
    int primes2[] = {2, 3, 5};
    int n2 = 10;
    int size2 = sizeof(primes2) / sizeof(primes2[0]);
    printf("%d\n", nthSuperUglyNumber(n2, primes2, size2));
    
    return 0;
}