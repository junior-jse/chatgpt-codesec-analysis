#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleList;

typedef struct {
    Tuple *elements;
    size_t count;
    size_t capacity;
} SingleElements;

static int safe_memcpy(void *dest, size_t dest_size, const void *src, size_t count) {
    if (count == 0) {
        return 0;
    }
    if (dest == NULL || src == NULL) {
        return -1;
    }
    if (dest_size == 0 || count > dest_size) {
        return -1;
    }
#if defined(__STDC_LIB_EXT1__)
    if (memcpy_s(dest, (rsize_t)dest_size, src, (rsize_t)count) != 0) {
        return -1;
    }
#else
    {
        unsigned char *d = (unsigned char *)dest;
        const unsigned char *s = (const unsigned char *)src;
        size_t i;

        if (dest == src) {
            return 0;
        }
        for (i = 0; i < count; i++) {
            d[i] = s[i];
        }
    }
#endif
    return 0;
}

static int compare_tuples(const void *a, const void *b) {
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;

    if (ta->size != tb->size) {
        return (ta->size < tb->size) ? -1 : 1;
    }

    for (size_t i = 0; i < ta->size; i++) {
        if (ta->data[i] != tb->data[i]) {
            return (ta->data[i] < tb->data[i]) ? -1 : 1;
        }
    }

    return 0;
}

static int tuple_equals(const Tuple *a, const Tuple *b) {
    if (a->size != b->size) {
        return 0;
    }

    for (size_t i = 0; i < a->size; i++) {
        if (a->data[i] != b->data[i]) {
            return 0;
        }
    }

    return 1;
}

static int add_to_singles(SingleElements *singles, const Tuple *tuple) {
    if (singles == NULL || tuple == NULL || (tuple->data == NULL && tuple->size > 0)) {
        return -1;
    }

    if (tuple->size > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    if (singles->count >= singles->capacity) {
        size_t new_capacity;
        Tuple *new_elements;

        if (singles->capacity == 0) {
            new_capacity = 8;
        } else {
            if (singles->capacity > SIZE_MAX / 2) {
                return -1;
            }
            new_capacity = singles->capacity * 2;
        }

        if (new_capacity < singles->capacity || new_capacity > SIZE_MAX / sizeof(Tuple)) {
            return -1;
        }

        new_elements = realloc(singles->elements, new_capacity * sizeof(Tuple));
        if (new_elements == NULL) {
            return -1;
        }
        singles->elements = new_elements;
        singles->capacity = new_capacity;
    }

    {
        int *new_data = NULL;
        size_t copy_size = tuple->size * sizeof(int);

        if (tuple->size > 0) {
            new_data = malloc(copy_size);
            if (new_data == NULL) {
                return -1;
            }
            if (safe_memcpy(new_data, copy_size, tuple->data, copy_size) != 0) {
                free(new_data);
                return -1;
            }
        }

        singles->elements[singles->count].data = new_data;
        singles->elements[singles->count].size = tuple->size;
        singles->count++;
    }

    return 0;
}

SingleElements extract_single_occurrences(const TupleList *list) {
    SingleElements result = {NULL, 0, 0};
    Tuple *sorted;
    size_t copy_size;
    size_t i;

    if (list == NULL || list->tuples == NULL || list->count == 0) {
        return result;
    }

    if (list->count > SIZE_MAX / sizeof(Tuple)) {
        return result;
    }

    copy_size = list->count * sizeof(Tuple);

    sorted = malloc(copy_size);
    if (sorted == NULL) {
        return result;
    }

    if (safe_memcpy(sorted, copy_size, list->tuples, copy_size) != 0) {
        free(sorted);
        return result;
    }

    qsort(sorted, list->count, sizeof(Tuple), compare_tuples);

    i = 0;
    while (i < list->count) {
        size_t j = i;
        while (j < list->count && tuple_equals(&sorted[i], &sorted[j])) {
            j++;
        }

        if (j - i == 1) {
            if (add_to_singles(&result, &sorted[i]) != 0) {
                for (size_t k = 0; k < result.count; k++) {
                    free(result.elements[k].data);
                }
                free(result.elements);
                free(sorted);
                result.elements = NULL;
                result.count = 0;
                result.capacity = 0;
                return result;
            }
        }

        i = j;
    }

    free(sorted);
    return result;
}

void free_single_elements(SingleElements *singles) {
    if (singles == NULL) {
        return;
    }

    for (size_t i = 0; i < singles->count; i++) {
        free(singles->elements[i].data);
    }
    free(singles->elements);
    singles->elements = NULL;
    singles->count = 0;
    singles->capacity = 0;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        free(list->tuples[i].data);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

int create_tuple(Tuple *tuple, const int *data, size_t size) {
    size_t copy_size;

    if (tuple == NULL || (data == NULL && size > 0)) {
        return -1;
    }

    if (size > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    tuple->data = NULL;
    tuple->size = 0;

    copy_size = size * sizeof(int);

    if (size > 0) {
        tuple->data = malloc(copy_size);
        if (tuple->data == NULL) {
            return -1;
        }
        if (safe_memcpy(tuple->data, copy_size, data, copy_size) != 0) {
            free(tuple->data);
            tuple->data = NULL;
            return -1;
        }
    }
    tuple->size = size;

    return 0;
}

int main(void) {
    TupleList list = {NULL, 0};
    list.count = 6;
    list.tuples = calloc(list.count, sizeof(Tuple));

    if (list.tuples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    {
        const int data0[] = {1, 2, 3};
        const int data1[] = {4, 5};
        const int data2[] = {1, 2, 3};
        const int data3[] = {6};
        const int data4[] = {4, 5};
        const int data5[] = {7, 8, 9};

        if (create_tuple(&list.tuples[0], data0, 3) != 0 ||
            create_tuple(&list.tuples[1], data1, 2) != 0 ||
            create_tuple(&list.tuples[2], data2, 3) != 0 ||
            create_tuple(&list.tuples[3], data3, 1) != 0 ||
            create_tuple(&list.tuples[4], data4, 2) != 0 ||
            create_tuple(&list.tuples[5], data5, 3) != 0) {
            fprintf(stderr, "Failed to create tuples\n");
            free_tuple_list(&list);
            return EXIT_FAILURE;
        }
    }

    {
        SingleElements singles = extract_single_occurrences(&list);

        printf("Elements occurring singly:\n");
        for (size_t i = 0; i < singles.count; i++) {
            printf("(");
            for (size_t j = 0; j < singles.elements[i].size; j++) {
                printf("%d", singles.elements[i].data[j]);
                if (j < singles.elements[i].size - 1) {
                    printf(", ");
                }
            }
            printf(")\n");
        }

        free_single_elements(&singles);
    }

    free_tuple_list(&list);

    return EXIT_SUCCESS;
}