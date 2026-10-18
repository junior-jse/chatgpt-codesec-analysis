#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sift_down(int *heap, int start, int end) {
    int root = start;
    while (2 * root + 1 <= end) {
        int child = 2 * root + 1;
        int swap = root;
        if (heap[swap] < heap[child]) {
            swap = child;
        }
        if (child + 1 <= end && heap[swap] < heap[child + 1]) {
            swap = child + 1;
        }
        if (swap == root) {
            return;
        } else {
            int temp = heap[root];
            heap[root] = heap[swap];
            heap[swap] = temp;
            root = swap;
        }
    }
}

void heapify(int *list, size_t size) {
    if (size < 2) {
        return;
    }
    int end = (int)size - 1;
    int start = (end - 1) / 2;
    while (start >= 0) {
        sift_down(list, start, end);
        start--;
    }
}

int main(void) {
    size_t n = 0;
    size_t capacity = 16;
    int *list = malloc(capacity * sizeof(int));
    if (list == NULL) {
        return 1;
    }
    int val;
    while (scanf("%d", &val) == 1) {
        if (n == capacity) {
            if (capacity > SIZE_MAX / 2) {
                break;
            }
            capacity *= 2;
            int *temp = realloc(list, capacity * sizeof(int));
            if (temp == NULL) {
                free(list);
                return 1;
            }
            list = temp;
        }
        list[n++] = val;
    }
    heapify(list, n);
    for (size_t i = 0; i < n; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
    free(list);
    return 0;
}