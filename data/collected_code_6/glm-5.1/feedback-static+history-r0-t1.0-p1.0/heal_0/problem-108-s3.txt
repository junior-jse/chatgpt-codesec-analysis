#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *values;
    int length;
    int index;
} Input;

typedef struct {
    Input *inputs;
    int heap_size;
    int capacity;
} MergeIterator;

void swap_inputs(Input *a, Input *b) {
    Input temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MergeIterator *it, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (it->inputs[index].values[it->inputs[index].index] < it->inputs[parent].values[it->inputs[parent].index]) {
            swap_inputs(&it->inputs[index], &it->inputs[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MergeIterator *it, int index) {
    while (1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < it->heap_size && it->inputs[left].values[it->inputs[left].index] < it->inputs[smallest].values[it->inputs[smallest].index]) {
            smallest = left;
        }

        if (right < it->heap_size && it->inputs[right].values[it->inputs[right].index] < it->inputs[smallest].values[it->inputs[smallest].index]) {
            smallest = right;
        }

        if (smallest != index) {
            swap_inputs(&it->inputs[index], &it->inputs[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

MergeIterator *merge_iterator_create(Input *inputs, int count) {
    if (inputs == NULL || count <= 0) {
        return NULL;
    }

    MergeIterator *it = (MergeIterator *)malloc(sizeof(MergeIterator));
    if (it == NULL) {
        return NULL;
    }

    it->capacity = count;
    it->heap_size = 0;
    it->inputs = (Input *)malloc(sizeof(Input) * count);
    if (it->inputs == NULL) {
        free(it);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        if (inputs[i].values != NULL && inputs[i].length > 0) {
            it->inputs[it->heap_size] = inputs[i];
            it->inputs[it->heap_size].index = 0;
            it->heap_size++;
            heapify_up(it, it->heap_size - 1);
        }
    }

    if (it->heap_size == 0) {
        free(it->inputs);
        free(it);
        return NULL;
    }

    return it;
}

bool merge_iterator_has_next(MergeIterator *it) {
    if (it == NULL) {
        return false;
    }
    return it->heap_size > 0;
}

int merge_iterator_next(MergeIterator *it) {
    if (it == NULL || it->heap_size == 0) {
        return 0;
    }

    int val = it->inputs[0].values[it->inputs[0].index];

    it->inputs[0].index++;
    if (it->inputs[0].index < it->inputs[0].length) {
        heapify_down(it, 0);
    } else {
        it->inputs[0] = it->inputs[it->heap_size - 1];
        it->heap_size--;
        if (it->heap_size > 0) {
            heapify_down(it, 0);
        }
    }

    return val;
}

void merge_iterator_destroy(MergeIterator *it) {
    if (it != NULL) {
        if (it->inputs != NULL) {
            free(it->inputs);
        }
        free(it);
    }
}

int main(void) {
    int a[] = {1, 5, 9};
    int b[] = {2, 4, 8};
    int c[] = {3, 6, 7};

    Input inputs[] = {
        {a, 3, 0},
        {b, 3, 0},
        {c, 3, 0}
    };

    MergeIterator *it = merge_iterator_create(inputs, 3);
    if (it == NULL) {
        return EXIT_FAILURE;
    }

    while (merge_iterator_has_next(it)) {
        printf("%d ", merge_iterator_next(it));
    }
    printf("\n");

    merge_iterator_destroy(it);

    return EXIT_SUCCESS;
}