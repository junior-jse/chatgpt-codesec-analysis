#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    long value;
    int prime_index;
    int next_factor_index;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_min_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_heap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_down(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < heap->size && heap->data[left].value < heap->data[smallest].value) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].value < heap->data[smallest].value) {
        smallest = right;
    }
    
    if (smallest != index) {
        swap_heap_nodes(&heap->data[index], &heap->data[smallest]);
        heapify_down(heap, smallest);
    }
}

void heapify_up(MinHeap* heap, int index) {
    int parent = (index - 1) / 2;
    
    while (index > 0 && heap->data[parent].value > heap->data[index].value) {
        swap_heap_nodes(&heap->data[parent], &heap->data[index]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void heap_push(MinHeap* heap, HeapNode node) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size] = node;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) {
        heapify_down(heap, 0);
    }
    return root;
}

HeapNode heap_peek(MinHeap* heap) {
    return heap->data[0];
}

void free_min_heap(MinHeap* heap) {
    if (heap) {
        if (heap->data) {
            free(heap->data);
        }
        free(heap);
    }
}

int nth_super_ugly_number(int n, int* primes, int primes_size) {
    if (n <= 0 || primes_size <= 0 || !primes) {
        return 0;
    }
    
    if (n == 1) {
        return 1;
    }
    
    long* ugly_numbers = (long*)malloc(sizeof(long) * n);
    if (!ugly_numbers) {
        return 0;
    }
    
    ugly_numbers[0] = 1;
    
    MinHeap* heap = create_min_heap(primes_size);
    if (!heap) {
        free(ugly_numbers);
        return 0;
    }
    
    for (int i = 0; i < primes_size; i++) {
        HeapNode node;
        node.value = (long)primes[i];
        node.prime_index = i;
        node.next_factor_index = 0;
        heap_push(heap, node);
    }
    
    for (int i = 1; i < n; i++) {
        HeapNode min_node = heap_peek(heap);
        ugly_numbers[i] = min_node.value;
        
        while (heap->size > 0 && heap_peek(heap).value == min_node.value) {
            HeapNode node = heap_pop(heap);
            node.next_factor_index++;
            node.value = (long)primes[node.prime_index] * ugly_numbers[node.next_factor_index];
            heap_push(heap, node);
        }
    }
    
    int result = (int)ugly_numbers[n - 1];
    
    free(ugly_numbers);
    free_min_heap(heap);
    
    return result;
}

int main() {
    int primes1[] = {2, 7, 13, 19};
    int primes1_size = sizeof(primes1) / sizeof(primes1[0]);
    int n1 = 12;
    printf("The %dth super ugly number is: %d\n", n1, nth_super_ugly_number(n1, primes1, primes1_size));
    
    int primes2[] = {2, 3, 5};
    int primes2_size = sizeof(primes2) / sizeof(primes2[0]);
    int n2 = 10;
    printf("The %dth super ugly number is: %d\n", n2, nth_super_ugly_number(n2, primes2, primes2_size));
    
    int primes3[] = {3, 5, 7, 11, 13};
    int primes3_size = sizeof(primes3) / sizeof(primes3[0]);
    int n3 = 20;
    printf("The %dth super ugly number is: %d\n", n3, nth_super_ugly_number(n3, primes3, primes3_size));
    
    int primes4[] = {2, 3, 5, 7};
    int primes4_size = sizeof(primes4) / sizeof(primes4[0]);
    int n4 = 1;
    printf("The %dth super ugly number is: %d\n", n4, nth_super_ugly_number(n4, primes4, primes4_size));
    
    int primes5[] = {5, 7, 13, 17, 19, 23, 29, 31, 37, 41};
    int primes5_size = sizeof(primes5) / sizeof(primes5[0]);
    int n5 = 35;
    printf("The %dth super ugly number is: %d\n", n5, nth_super_ugly_number(n5, primes5, primes5_size));
    
    return 0;
}