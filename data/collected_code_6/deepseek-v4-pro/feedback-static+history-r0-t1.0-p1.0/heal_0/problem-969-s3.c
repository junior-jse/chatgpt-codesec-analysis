#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple *data;
    size_t size;
    size_t capacity;
} TupleList;

static void init_list(TupleList *list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

static int push_tuple(TupleList *list, int first, int second) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 8 : list->capacity * 2;
        Tuple *new_data = realloc(list->data, new_capacity * sizeof(Tuple));
        if (!new_data) return 0;
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size].first = first;
    list->data[list->size].second = second;
    list->size++;
    return 1;
}

static void free_list(TupleList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

static int cmp_tuple(const void *a, const void *b) {
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;
    if (ta->first < tb->first) return -1;
    if (ta->first > tb->first) return 1;
    if (ta->second < tb->second) return -1;
    if (ta->second > tb->second) return 1;
    return 0;
}

TupleList join_tuples(const Tuple *input, size_t count) {
    TupleList result;
    init_list(&result);

    if (count == 0 || !input) {
        return result;
    }

    Tuple *sorted = malloc(count * sizeof(Tuple));
    if (!sorted) return result;

    for (size_t i = 0; i < count; i++) {
        sorted[i] = input[i];
    }

    qsort(sorted, count, sizeof(Tuple), cmp_tuple);

    size_t i = 0;
    while (i < count) {
        int first = sorted[i].first;
        int second_sum = sorted[i].second;
        size_t j = i + 1;

        while (j < count && sorted[j].first == first) {
            second_sum += sorted[j].second;
            j++;
        }

        if (!push_tuple(&result, first, second_sum)) {
            free(sorted);
            free_list(&result);
            init_list(&result);
            return result;
        }

        i = j;
    }

    free(sorted);
    return result;
}

int main(void) {
    Tuple input[] = {
        {1, 2}, {2, 3}, {1, 4}, {3, 1}, {2, 5}, {1, 6}
    };
    size_t count = sizeof(input) / sizeof(input[0]);

    TupleList result = join_tuples(input, count);

    if (result.data == NULL && count != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result.size; i++) {
        printf("(%d, %d)\n", result.data[i].first, result.data[i].second);
    }

    free_list(&result);
    return EXIT_SUCCESS;
}