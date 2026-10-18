#include <stdio.h>
#include <stdlib.h>

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

void heapify(int *heap, int size) {
    if (size < 2) {
        return;
    }
    int end = size - 1;
    int start = (end - 1) / 2;
    while (start >= 0) {
        sift_down(heap, start, end);
        start--;
    }
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    int size = argc - 1;
    int *heap = (int *)malloc(size * sizeof(int));
    if (heap == NULL) {
        return 1;
    }
    for (int i = 0; i < size; i++) {
        /* Possible weaknesses found:
         * Flawfinder atoi: Unless checked, the resulting number can exceed the expected range (CWE-190). If source untrusted, check both minimum and maximum, even if the input had no minus sign (large numbers can roll over into negative number; consider saving to an unsigned value if that is intended). (risk 2, integer)
         */
        heap[i] = atoi(argv[i + 1]);
    }
    heapify(heap, size);
    for (int i = 0; i < size; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");
    free(heap);
    return 0;
}