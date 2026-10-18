#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int count;
} Pair;

typedef struct {
    Pair *data;
    int size;
    int capacity;
} MinHeap;

static void swap(Pair *a, Pair *b) {
    Pair temp = *a;
    *a = *b;
    *b = temp;
}

static void minHeapInit(MinHeap *heap, int capacity) {
    heap->data = malloc(sizeof(Pair) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
}

static void minHeapifyDown(MinHeap *heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left].count < heap->data[smallest].count) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].count < heap->data[smallest].count) {
        smallest = right;
    }
    if (smallest != idx) {
        swap(&heap->data[idx], &heap->data[smallest]);
        minHeapifyDown(heap, smallest);
    }
}

static void minHeapifyUp(MinHeap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent].count > heap->data[idx].count) {
            swap(&heap->data[parent], &heap->data[idx]);
            idx = parent;
        } else {
            break;
        }
    }
}

static void minHeapPush(MinHeap *heap, Pair item) {
    if (heap->size < heap->capacity) {
        heap->data[heap->size] = item;
        heap->size++;
        minHeapifyUp(heap, heap->size - 1);
    } else if (item.count > heap->data[0].count) {
        heap->data[0] = item;
        minHeapifyDown(heap, 0);
    }
}

static Pair minHeapPop(MinHeap *heap) {
    Pair top = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) {
        minHeapifyDown(heap, 0);
    }
    return top;
}

static void minHeapFree(MinHeap *heap) {
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

static int cmpDesc(const void *a, const void *b) {
    const Pair *pa = (const Pair *)a;
    const Pair *pb = (const Pair *)b;
    if (pb->count != pa->count) {
        return pb->count - pa->count;
    }
    return pb->value - pa->value;
}

int* topKFrequent(int** lists, int listsSize, const int* listSizes, int k, int* returnSize) {
    int total = 0;
    for (int i = 0; i < listsSize; i++) {
        total += listSizes[i];
    }

    int *all = malloc(sizeof(int) * total);
    int idx = 0;
    for (int i = 0; i < listsSize; i++) {
        for (int j = 0; j < listSizes[i]; j++) {
            all[idx++] = lists[i][j];
        }
    }

    for (int i = 1; i < total; i++) {
        int key = all[i];
        int j = i - 1;
        while (j >= 0 && all[j] > key) {
            all[j + 1] = all[j];
            j--;
        }
        all[j + 1] = key;
    }

    MinHeap heap;
    minHeapInit(&heap, k);

    int i = 0;
    while (i < total) {
        int j = i;
        while (j < total && all[j] == all[i]) {
            j++;
        }
        Pair p;
        p.value = all[i];
        p.count = j - i;
        minHeapPush(&heap, p);
        i = j;
    }

    int *result = malloc(sizeof(int) * k);
    int resultIdx = 0;
    Pair *temp = malloc(sizeof(Pair) * heap.size);
    int tempSize = heap.size;
    for (int t = 0; t < tempSize; t++) {
        temp[t] = minHeapPop(&heap);
    }

    qsort(temp, tempSize, sizeof(Pair), cmpDesc);

    for (int t = 0; t < k; t++) {
        result[resultIdx++] = temp[t].value;
    }

    free(temp);
    free(all);
    minHeapFree(&heap);
    *returnSize = k;
    return result;
}

int main(void) {
    int list0[] = {1, 3, 5};
    int list1[] = {2, 3, 6};
    int list2[] = {3, 5, 6};
    int list3[] = {1, 2, 3};

    int *lists[] = {list0, list1, list2, list3};
    /* Possible weaknesses found:
     *  Variable 'listSizes' can be declared as const array [constVariable]
     */
    int listSizes[] = {3, 3, 3, 3};
    int listsSize = 4;
    int k = 3;
    int returnSize = 0;

    int *result = topKFrequent(lists, listsSize, listSizes, k, &returnSize);

    printf("[");
    for (int i = 0; i < returnSize; i++) {
        if (i > 0) printf(", ");
        printf("%d", result[i]);
    }
    printf("]\n");

    free(result);
    return 0;
}