#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void push(int *heap, int *size, int value) {
    heap[*size] = value;
    int i = *size;
    while (i > 0 && heap[(i - 1) / 2] > heap[i]) {
        swap(&heap[(i - 1) / 2], &heap[i]);
        i = (i - 1) / 2;
    }
    (*size)++;
}

int pop(int *heap, int *size) {
    if (*size == 0) {
        fprintf(stderr, "Heap underflow\n");
        exit(EXIT_FAILURE);
    }
    int min_val = heap[0];
    heap[0] = heap[*size - 1];
    (*size)--;
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < *size && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < *size && heap[right] < heap[smallest]) {
            smallest = right;
        }
        if (smallest == i) {
            break;
        }
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return min_val;
}

void heap_sort(int *values, int count) {
    if (count <= 0) {
        return;
    }
    int *heap = malloc(count * sizeof(int));
    if (heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int heap_size = 0;
    for (int i = 0; i < count; i++) {
        push(heap, &heap_size, values[i]);
    }
    for (int i = 0; i < count; i++) {
        values[i] = pop(heap, &heap_size);
    }
    free(heap);
}

int main(void) {
    int values[] = {12, 11, 13, 5, 6, 7};
    int count = sizeof(values) / sizeof(values[0]);
    heap_sort(values, count);
    for (int i = 0; i < count; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");
    return 0;
}