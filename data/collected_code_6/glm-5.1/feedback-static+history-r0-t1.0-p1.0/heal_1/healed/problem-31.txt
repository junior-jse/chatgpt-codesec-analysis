#include <stdio.h>
#include <stdlib.h>

struct HeapNode {
    int value;
    int frequency;
};

void swap(struct HeapNode *a, struct HeapNode *b) {
    struct HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct HeapNode *heap, int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].frequency < heap[smallest].frequency) {
        smallest = left;
    }

    if (right < size && heap[right].frequency < heap[smallest].frequency) {
        smallest = right;
    }

    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        minHeapify(heap, size, smallest);
    }
}

int* topKFrequent(int **lists, const int *listSizes, int numLists, int k, int *returnSize) {
    struct HeapNode *heap = (struct HeapNode *)malloc(k * sizeof(struct HeapNode));
    if (!heap) {
        *returnSize = 0;
        return NULL;
    }

    int *result = (int *)malloc(k * sizeof(int));
    if (!result) {
        free(heap);
        *returnSize = 0;
        return NULL;
    }

    int *indices = (int *)calloc(numLists, sizeof(int));
    if (!indices) {
        free(heap);
        free(result);
        *returnSize = 0;
        return NULL;
    }

    int heapSize = 0;
    int currentVal = 0;
    int firstIteration = 1;

    while (1) {
        int minVal = 0;
        int found = 0;

        for (int i = 0; i < numLists; i++) {
            if (indices[i] < listSizes[i]) {
                if (!found || lists[i][indices[i]] < minVal) {
                    minVal = lists[i][indices[i]];
                    found = 1;
                }
            }
        }

        if (!found) {
            break;
        }

        if (firstIteration || minVal != currentVal) {
            currentVal = minVal;
            int currentFreq = 0;

            for (int i = 0; i < numLists; i++) {
                while (indices[i] < listSizes[i] && lists[i][indices[i]] == currentVal) {
                    currentFreq++;
                    indices[i]++;
                }
            }

            if (heapSize < k) {
                heap[heapSize].value = currentVal;
                heap[heapSize].frequency = currentFreq;
                heapSize++;
                if (heapSize == k) {
                    for (int j = (k / 2) - 1; j >= 0; j--) {
                        minHeapify(heap, k, j);
                    }
                }
            } else if (currentFreq > heap[0].frequency) {
                heap[0].value = currentVal;
                heap[0].frequency = currentFreq;
                minHeapify(heap, k, 0);
            }

            firstIteration = 0;
        }
    }

    for (int i = 0; i < heapSize; i++) {
        result[i] = heap[i].value;
    }

    *returnSize = heapSize;

    free(heap);
    free(indices);

    return result;
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {2, 3, 4, 5};
    int list3[] = {1, 3, 5, 6};

    int *lists[] = {list1, list2, list3};
    int listSizes[] = {3, 4, 4};
    int numLists = 3;
    int k = 2;
    int returnSize = 0;

    int *topK = topKFrequent(lists, listSizes, numLists, k, &returnSize);

    if (topK) {
        for (int i = 0; i < returnSize; i++) {
            printf("%d ", topK[i]);
        }
        printf("\n");
        free(topK);
    }

    return 0;
}