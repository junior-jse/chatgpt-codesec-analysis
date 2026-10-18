#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int count;
} Element;

typedef struct {
    Element *data;
    int capacity;
    int size;
} MinHeap;

MinHeap* create_min_heap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (Element*)malloc(capacity * sizeof(Element));
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void destroy_min_heap(MinHeap *heap) {
    if (heap != NULL) {
        free(heap->data);
        free(heap);
    }
}

void swap_elements(Element *a, Element *b) {
    Element temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index].count >= heap->data[parent].count) {
            break;
        }
        swap_elements(&heap->data[index], &heap->data[parent]);
        index = parent;
    }
}

void heapify_down(MinHeap *heap, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap->size && heap->data[left].count < heap->data[smallest].count) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].count < heap->data[smallest].count) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        swap_elements(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

void push_min_heap(MinHeap *heap, Element elem) {
    if (heap->size == heap->capacity) {
        if (elem.count > heap->data[0].count) {
            heap->data[0] = elem;
            heapify_down(heap, 0);
        }
        return;
    }
    heap->data[heap->size] = elem;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

Element pop_min_heap(MinHeap *heap) {
    Element top = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return top;
}

int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

/* Possible weaknesses found:
 *  Parameter 'list_sizes' can be declared as pointer to const [constParameterPointer]
 */
int* top_k_frequent(int **lists, int *list_sizes, int num_lists, int k, int *return_size) {
    int total_elements = 0;
    for (int i = 0; i < num_lists; i++) {
        total_elements += list_sizes[i];
    }

    if (total_elements == 0 || k <= 0) {
        *return_size = 0;
        return NULL;
    }

    int *merged = (int*)malloc(total_elements * sizeof(int));
    if (merged == NULL) {
        *return_size = 0;
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < num_lists; i++) {
        for (int j = 0; j < list_sizes[i]; j++) {
            merged[index++] = lists[i][j];
        }
    }

    qsort(merged, total_elements, sizeof(int), compare_ints);

    MinHeap *heap = create_min_heap(k);
    if (heap == NULL) {
        free(merged);
        *return_size = 0;
        return NULL;
    }

    int current_val = merged[0];
    int current_count = 1;

    for (int i = 1; i < total_elements; i++) {
        if (merged[i] == current_val) {
            current_count++;
        } else {
            Element elem = {current_val, current_count};
            push_min_heap(heap, elem);
            current_val = merged[i];
            current_count = 1;
        }
    }
    
    Element last_elem = {current_val, current_count};
    push_min_heap(heap, last_elem);

    free(merged);

    *return_size = heap->size;
    int *result = (int*)malloc(heap->size * sizeof(int));
    if (result == NULL) {
        destroy_min_heap(heap);
        return NULL;
    }

    while (heap->size > 0) {
        Element top = pop_min_heap(heap);
        result[heap->size] = top.value;
    }

    destroy_min_heap(heap);
    return result;
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {2, 4};
    int list3[] = {1, 2, 5, 6};
    
    int *lists[] = {list1, list2, list3};
    int list_sizes[] = {3, 2, 4};
    int num_lists = 3;
    int k = 2;
    int return_size = 0;

    int *result = top_k_frequent(lists, list_sizes, num_lists, k, &return_size);
    
    for (int i = 0; i < return_size; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    free(result);
    return 0;
}