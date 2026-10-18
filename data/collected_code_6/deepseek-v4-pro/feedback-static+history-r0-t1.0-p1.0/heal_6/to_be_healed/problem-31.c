#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

static int cmpInt(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

int* topKFrequent(int** lists, int listsSize, const int* listSizes, int k, int* returnSize) {
    int total = 0;
    for (int i = 0; i < listsSize; i++) {
        if (listSizes[i] < 0) {
            *returnSize = 0;
            return NULL;
        }
        if (total > INT_MAX - listSizes[i]) {
            *returnSize = 0;
            return NULL;
        }
        total += listSizes[i];
    }

    if (total == 0) {
        *returnSize = 0;
        return NULL;
    }

    int *all = malloc(sizeof(int) * total);
    if (!all) {
        *returnSize = 0;
        return NULL;
    }

    int idx = 0;
    for (int i = 0; i < listsSize; i++) {
        if (listSizes[i] < 0 || idx > total - listSizes[i]) {
            free(all);
            *returnSize = 0;
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(all + idx, lists[i], sizeof(int) * listSizes[i]);
        idx += listSizes[i];
    }

    qsort(all, total, sizeof(int), cmpInt);

    MinHeap heap;
    minHeapInit(&heap, k);
    if (!heap.data) {
        free(all);
        *returnSize = 0;
        return NULL;
    }

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
    if (!result) {
        free(all);
        minHeapFree(&heap);
        *returnSize = 0;
        return NULL;
    }

    int resultIdx = 0;
    Pair *temp = malloc(sizeof(Pair) * heap.size);
    if (!temp) {
        free(result);
        free(all);
        minHeapFree(&heap);
        *returnSize = 0;
        return NULL;
    }

    int tempSize = heap.size;
    for (int t = 0; t < tempSize; t++) {
        temp[t] = minHeapPop(&heap);
    }

    qsort(temp, tempSize, sizeof(Pair), cmpDesc);

    int resultCount = tempSize < k ? tempSize : k;
    for (int t = 0; t < resultCount; t++) {
        result[resultIdx++] = temp[t].value;
    }

    free(temp);
    free(all);
    minHeapFree(&heap);
    *returnSize = resultCount;
    return result;
}

int main(void) {
    int list0[] = {1, 3, 5};
    int list1[] = {2, 3, 6};
    int list2[] = {3, 5, 6};
    int list3[] = {1, 2, 3};

    int *lists[] = {list0, list1, list2, list3};
    const int listSizes[] = {3, 3, 3, 3};
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