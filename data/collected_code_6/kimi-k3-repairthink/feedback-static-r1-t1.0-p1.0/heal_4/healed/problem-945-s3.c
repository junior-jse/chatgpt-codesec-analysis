#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    int *elements;
    size_t size;
} Set;

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

static int safe_copy_ints(int *dest, size_t dest_count, const int *src, size_t src_count) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    if (src_count == 0) {
        return 0;
    }
    if (dest_count > SIZE_MAX / sizeof(int) || src_count > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    size_t dest_bytes = dest_count * sizeof(int);
    size_t src_bytes = src_count * sizeof(int);
    if (src_bytes > dest_bytes) {
        return -1;
    }
#if defined(__STDC_LIB_EXT1__)
    if (memcpy_s(dest, (rsize_t)dest_bytes, src, (rsize_t)src_bytes) != 0) {
        return -1;
    }
#else
    memcpy(dest, src, src_bytes);
#endif
    return 0;
}

Set tuple_to_set(const Tuple *tuple) {
    Set result = {NULL, 0};

    if (tuple == NULL || tuple->elements == NULL || tuple->size == 0) {
        return result;
    }

    if (tuple->size > SIZE_MAX / sizeof(int)) {
        return result;
    }

    int *temp = malloc(tuple->size * sizeof(int));
    if (temp == NULL) {
        return result;
    }

    if (safe_copy_ints(temp, tuple->size, tuple->elements, tuple->size) != 0) {
        free(temp);
        return result;
    }

    qsort(temp, tuple->size, sizeof(int), compare_ints);

    size_t unique_count = 0;
    for (size_t i = 0; i < tuple->size; i++) {
        if (i == 0 || temp[i] != temp[i - 1]) {
            temp[unique_count++] = temp[i];
        }
    }

    if (unique_count == 0) {
        free(temp);
        return result;
    }

    if (unique_count > SIZE_MAX / sizeof(int)) {
        result.elements = temp;
        result.size = unique_count;
        return result;
    }

    int *final_elements = realloc(temp, unique_count * sizeof(int));
    if (final_elements == NULL) {
        result.elements = temp;
        result.size = unique_count;
    } else {
        result.elements = final_elements;
        result.size = unique_count;
    }

    return result;
}

void free_set(Set *set) {
    if (set != NULL) {
        free(set->elements);
        set->elements = NULL;
        set->size = 0;
    }
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        free(tuple->elements);
        tuple->elements = NULL;
        tuple->size = 0;
    }
}

int main(void) {
    int data[] = {5, 2, 8, 2, 9, 1, 5, 5, 8};
    size_t data_size = sizeof(data) / sizeof(data[0]);

    if (data_size > SIZE_MAX / sizeof(int)) {
        return EXIT_FAILURE;
    }

    Tuple input = {NULL, 0};
    input.elements = malloc(data_size * sizeof(int));
    if (input.elements == NULL) {
        return EXIT_FAILURE;
    }

    if (safe_copy_ints(input.elements, data_size, data, data_size) != 0) {
        free(input.elements);
        input.elements = NULL;
        return EXIT_FAILURE;
    }
    input.size = data_size;

    Set output = tuple_to_set(&input);

    if (output.elements != NULL) {
        printf("{ ");
        for (size_t i = 0; i < output.size; i++) {
            printf("%d ", output.elements[i]);
        }
        printf("}\n");
    }

    free_set(&output);
    free_tuple(&input);

    return EXIT_SUCCESS;
}