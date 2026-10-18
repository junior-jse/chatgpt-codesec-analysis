#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int frequency;
    int listIndex;
    int elementIndex;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
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

void swapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyDown(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left].frequency < heap->data[smallest].frequency)
        smallest = left;
    if (right < heap->size && heap->data[right].frequency < heap->data[smallest].frequency)
        smallest = right;

    if (smallest != index) {
        swapNodes(&heap->data[index], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

void heapifyUp(MinHeap* heap, int index) {
    int parent = (index - 1) / 2;
    if (index > 0 && heap->data[index].frequency < heap->data[parent].frequency) {
        swapNodes(&heap->data[index], &heap->data[parent]);
        heapifyUp(heap, parent);
    }
}

void pushHeap(MinHeap* heap, HeapNode node) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode popHeap(MinHeap* heap) {
    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return root;
}

void freeMinHeap(MinHeap* heap) {
    if (heap) {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int* topKFrequent(int** lists, int numLists, int* listSizes, int k, int* returnSize) {
    if (k <= 0 || numLists <= 0 || lists == NULL || listSizes == NULL || returnSize == NULL) {
        *returnSize = 0;
        return NULL;
    }

    int totalCapacity = 0;
    for (int i = 0; i < numLists; i++) {
        totalCapacity += listSizes[i];
    }

    if (k > totalCapacity) k = totalCapacity;

    MinHeap* heap = createMinHeap(k);
    if (!heap) {
        *returnSize = 0;
        return NULL;
    }

    int** currentPositions = (int**)malloc(sizeof(int*) * numLists);
    if (!currentPositions) {
        freeMinHeap(heap);
        *returnSize = 0;
        return NULL;
    }

    for (int i = 0; i < numLists; i++) {
        currentPositions[i] = lists[i];
        if (currentPositions[i] == NULL || listSizes[i] <= 0) {
            currentPositions[i] = NULL;
        }
    }

    while (1) {
        int minVal = 0;
        int minListIndex = -1;
        int found = 0;

        for (int i = 0; i < numLists; i++) {
            if (currentPositions[i] != NULL && currentPositions[i] - lists[i] < listSizes[i]) {
                if (!found || *currentPositions[i] < minVal) {
                    minVal = *currentPositions[i];
                    minListIndex = i;
                    found = 1;
                }
            }
        }

        if (!found) break;

        int count = 0;
        for (int i = 0; i < numLists; i++) {
            if (currentPositions[i] != NULL && currentPositions[i] - lists[i] < listSizes[i] && *currentPositions[i] == minVal) {
                count++;
                currentPositions[i]++;
            }
        }

        HeapNode node;
        node.value = minVal;
        node.frequency = count;
        node.listIndex = minListIndex;
        node.elementIndex = 0;

        if (heap->size < k) {
            pushHeap(heap, node);
        } else if (count > heap->data[0].frequency) {
            popHeap(heap);
            pushHeap(heap, node);
        }
    }

    *returnSize = heap->size;
    int* result = (int*)malloc(sizeof(int) * (*returnSize));
    if (!result) {
        free(currentPositions);
        freeMinHeap(heap);
        *returnSize = 0;
        return NULL;
    }

    int resultIndex = *returnSize - 1;
    while (heap->size > 0) {
        HeapNode node = popHeap(heap);
        result[resultIndex--] = node.value;
    }

    free(currentPositions);
    freeMinHeap(heap);
    return result;
}

int main() {
    int list1[] = {1, 2, 3, 4, 5};
    int list2[] = {2, 3, 4, 5, 6};
    int list3[] = {3, 4, 5, 6, 7};

    int* lists[] = {list1, list2, list3};
    int listSizes[] = {5, 5, 5};
    int numLists = 3;
    int k = 3;
    int returnSize = 0;

    int* result = topKFrequent(lists, numLists, listSizes, k, &returnSize);

    printf("Top %d most frequent integers: ", k);
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    free(result);
    return 0;
}