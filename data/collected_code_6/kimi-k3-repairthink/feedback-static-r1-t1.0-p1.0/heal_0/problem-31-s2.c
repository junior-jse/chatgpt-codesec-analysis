#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int frequency;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int size;
    int capacity;
} MinHeap;

static MinHeap* create_heap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }
    heap->nodes = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    if (heap->nodes == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

static void destroy_heap(MinHeap *heap) {
    if (heap != NULL) {
        free(heap->nodes);
        free(heap);
    }
}

static void swap_nodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify_up(MinHeap *heap, int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && heap->nodes[parent].frequency > heap->nodes[index].frequency) {
        swap_nodes(&heap->nodes[parent], &heap->nodes[index]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

static void heapify_down(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->nodes[left].frequency < heap->nodes[smallest].frequency) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].frequency < heap->nodes[smallest].frequency) {
        smallest = right;
    }
    if (smallest != index) {
        swap_nodes(&heap->nodes[index], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

static int heap_insert(MinHeap *heap, int value, int frequency) {
    if (heap->size >= heap->capacity) {
        return 0;
    }
    heap->nodes[heap->size].value = value;
    heap->nodes[heap->size].frequency = frequency;
    heapify_up(heap, heap->size);
    heap->size++;
    return 1;
}

static int heap_extract_min(MinHeap *heap, HeapNode *result) {
    if (heap->size <= 0) {
        return 0;
    }
    *result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return 1;
}

static int compare_nodes(const void *a, const void *b) {
    const HeapNode *node_a = (const HeapNode*)a;
    const HeapNode *node_b = (const HeapNode*)b;
    if (node_a->frequency != node_b->frequency) {
        return node_b->frequency - node_a->frequency;
    }
    return node_a->value - node_b->value;
}

int* top_k_frequent(const int *nums, int nums_size, int k, int *return_size) {
    if (nums == NULL || nums_size <= 0 || k <= 0 || return_size == NULL) {
        if (return_size != NULL) {
            *return_size = 0;
        }
        return NULL;
    }

    HeapNode *unique_nodes = (HeapNode*)malloc(sizeof(HeapNode) * nums_size);
    if (unique_nodes == NULL) {
        *return_size = 0;
        return NULL;
    }

    int unique_count = 0;
    int i = 0;
    while (i < nums_size) {
        int current_value = nums[i];
        int count = 1;
        i++;
        while (i < nums_size && nums[i] == current_value) {
            count++;
            i++;
        }
        unique_nodes[unique_count].value = current_value;
        unique_nodes[unique_count].frequency = count;
        unique_count++;
    }

    int actual_k = (k < unique_count) ? k : unique_count;
    MinHeap *heap = create_heap(actual_k);
    if (heap == NULL) {
        free(unique_nodes);
        *return_size = 0;
        return NULL;
    }

    for (i = 0; i < unique_count; i++) {
        if (heap->size < actual_k) {
            if (!heap_insert(heap, unique_nodes[i].value, unique_nodes[i].frequency)) {
                destroy_heap(heap);
                free(unique_nodes);
                *return_size = 0;
                return NULL;
            }
        } else if (unique_nodes[i].frequency > heap->nodes[0].frequency) {
            HeapNode temp;
            if (heap_extract_min(heap, &temp)) {
                if (!heap_insert(heap, unique_nodes[i].value, unique_nodes[i].frequency)) {
                    destroy_heap(heap);
                    free(unique_nodes);
                    *return_size = 0;
                    return NULL;
                }
            }
        }
    }

    int *result = (int*)malloc(sizeof(int) * actual_k);
    if (result == NULL) {
        destroy_heap(heap);
        free(unique_nodes);
        *return_size = 0;
        return NULL;
    }

    HeapNode *temp_array = (HeapNode*)malloc(sizeof(HeapNode) * actual_k);
    if (temp_array == NULL) {
        free(result);
        destroy_heap(heap);
        free(unique_nodes);
        *return_size = 0;
        return NULL;
    }

    int result_count = 0;
    HeapNode node;
    while (heap_extract_min(heap, &node)) {
        temp_array[result_count++] = node;
    }

    qsort(temp_array, result_count, sizeof(HeapNode), compare_nodes);

    for (i = 0; i < result_count; i++) {
        result[i] = temp_array[i].value;
    }

    *return_size = result_count;

    free(temp_array);
    destroy_heap(heap);
    free(unique_nodes);

    return result;
}

int main(void) {
    int nums[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 5, 6};
    int nums_size = sizeof(nums) / sizeof(nums[0]);
    int k = 3;
    int return_size = 0;

    int *result = top_k_frequent(nums, nums_size, k, &return_size);
    
    if (result == NULL) {
        fprintf(stderr, "Error: Failed to compute top k frequent elements\n");
        return EXIT_FAILURE;
    }

    printf("Top %d most frequent integers: ", k);
    for (int i = 0; i < return_size; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    free(result);
    return EXIT_SUCCESS;
}