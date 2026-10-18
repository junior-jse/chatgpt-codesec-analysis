#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long value;
    int prime;
    int index;
} HeapNode;

typedef struct {
    HeapNode *data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapNodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < heap->size && heap->data[left].value < heap->data[smallest].value)
        smallest = left;
    if (right < heap->size && heap->data[right].value < heap->data[smallest].value)
        smallest = right;
    
    if (smallest != index) {
        swapNodes(&heap->data[index], &heap->data[smallest]);
        heapify(heap, smallest);
    }
}

void insertHeap(MinHeap *heap, HeapNode node) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->data = (HeapNode*)realloc(heap->data, sizeof(HeapNode) * heap->capacity);
    }
    
    int index = heap->size;
    heap->data[index] = node;
    heap->size++;
    
    while (index > 0 && heap->data[(index - 1) / 2].value > heap->data[index].value) {
        swapNodes(&heap->data[index], &heap->data[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

HeapNode extractMin(MinHeap *heap) {
    HeapNode minNode = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return minNode;
}

void freeHeap(MinHeap *heap) {
    free(heap->data);
    free(heap);
}

int nthSuperUglyNumber(int n, int *primes, int primesSize) {
    if (n <= 0 || primesSize <= 0)
        return 0;
    
    int *ugly = (int*)malloc(sizeof(int) * n);
    MinHeap *heap = createMinHeap(primesSize);
    
    ugly[0] = 1;
    
    for (int i = 0; i < primesSize; i++) {
        HeapNode node = {primes[i], primes[i], 0};
        insertHeap(heap, node);
    }
    
    for (int i = 1; i < n; i++) {
        HeapNode minNode = extractMin(heap);
        ugly[i] = (int)minNode.value;
        
        long nextValue = (long)minNode.prime * ugly[minNode.index + 1];
        HeapNode nextNode = {nextValue, minNode.prime, minNode.index + 1};
        insertHeap(heap, nextNode);
        
        while (heap->size > 0 && heap->data[0].value == minNode.value) {
            HeapNode duplicate = extractMin(heap);
            long dupNextValue = (long)duplicate.prime * ugly[duplicate.index + 1];
            HeapNode dupNextNode = {dupNextValue, duplicate.prime, duplicate.index + 1};
            insertHeap(heap, dupNextNode);
        }
    }
    
    int result = ugly[n - 1];
    free(ugly);
    freeHeap(heap);
    return result;
}

int main(void) {
    int primes[] = {2, 7, 13, 19};
    int n = 12;
    int primesSize = sizeof(primes) / sizeof(primes[0]);
    
    int result = nthSuperUglyNumber(n, primes, primesSize);
    printf("The %dth super ugly number is: %d\n", n, result);
    
    return 0;
}