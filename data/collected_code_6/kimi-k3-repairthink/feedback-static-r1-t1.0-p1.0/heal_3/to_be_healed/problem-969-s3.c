#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
    size_t capacity;
} TupleList;

void init_tuple_list(TupleList *list) {
    list->tuples = NULL;
    list->size = 0;
    list->capacity = 0;
}

int add_tuple(TupleList *list, const int *elements, size_t size) {
    if (list == NULL || (elements == NULL && size > 0)) {
        return -1;
    }

    if (size > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    if (list->size == list->capacity) {
        size_t new_capacity;
        if (list->capacity == 0) {
            new_capacity = 4;
        } else {
            if (list->capacity > SIZE_MAX / 2) {
                return -1;
            }
            new_capacity = list->capacity * 2;
        }
        if (new_capacity > SIZE_MAX / sizeof(Tuple)) {
            return -1;
        }
        Tuple *new_tuples = realloc(list->tuples, new_capacity * sizeof(Tuple));
        if (new_tuples == NULL) {
            return -1;
        }
        list->tuples = new_tuples;
        list->capacity = new_capacity;
    }

    int *new_elements = NULL;
    if (size > 0) {
        /* Possible weaknesses found:
         *  dest_capacity is assigned 'size*sizeof(int)' here.
         */
        size_t dest_capacity = size * sizeof(int);
        new_elements = malloc(dest_capacity);
        /* Possible weaknesses found:
         *  Assuming condition is false
         */
        if (new_elements == NULL) {
            return -1;
        }
        /* Possible weaknesses found:
         *  copy_size is assigned 'size*sizeof(int)' here.
         */
        size_t copy_size = size * sizeof(int);
        /* Possible weaknesses found:
         *  Condition 'copy_size>dest_capacity' is always false [knownConditionTrueFalse]
         *  Condition 'copy_size>dest_capacity' is always false
         */
        if (copy_size > dest_capacity) {
            free(new_elements);
            return -1;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(new_elements, elements, copy_size);
    }

    list->tuples[list->size].elements = new_elements;
    list->tuples[list->size].size = size;
    list->size++;
    return 0;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->size; i++) {
        free(list->tuples[i].elements);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->size = 0;
    list->capacity = 0;
}

int join_tuples_by_initial(const TupleList *input, TupleList *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }

    init_tuple_list(output);

    int *processed = calloc(input->size, sizeof(int));
    if (processed == NULL) {
        return -1;
    }

    for (size_t i = 0; i < input->size; i++) {
        if (processed[i] || input->tuples[i].size == 0) {
            continue;
        }

        int initial = input->tuples[i].elements[0];
        size_t total_size = 0;

        for (size_t j = i; j < input->size; j++) {
            if (!processed[j] && input->tuples[j].size > 0 &&
                input->tuples[j].elements[0] == initial) {
                if (input->tuples[j].size > SIZE_MAX - total_size) {
                    free(processed);
                    free_tuple_list(output);
                    return -1;
                }
                total_size += input->tuples[j].size;
            }
        }

        if (total_size == 0 || total_size > SIZE_MAX / sizeof(int)) {
            free(processed);
            free_tuple_list(output);
            return -1;
        }

        size_t joined_capacity = total_size * sizeof(int);
        int *joined = malloc(joined_capacity);
        if (joined == NULL) {
            free(processed);
            free_tuple_list(output);
            return -1;
        }

        size_t offset = 0;
        for (size_t j = i; j < input->size; j++) {
            if (!processed[j] && input->tuples[j].size > 0 &&
                input->tuples[j].elements[0] == initial) {
                if (offset > total_size ||
                    input->tuples[j].size > total_size - offset) {
                    free(joined);
                    free(processed);
                    free_tuple_list(output);
                    return -1;
                }
                size_t copy_size = input->tuples[j].size * sizeof(int);
                size_t dest_remaining = joined_capacity - offset * sizeof(int);
                if (copy_size > dest_remaining) {
                    free(joined);
                    free(processed);
                    free_tuple_list(output);
                    return -1;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(joined + offset, input->tuples[j].elements, copy_size);
                offset += input->tuples[j].size;
                processed[j] = 1;
            }
        }

        if (add_tuple(output, joined, total_size) != 0) {
            free(joined);
            free(processed);
            free_tuple_list(output);
            return -1;
        }

        free(joined);
    }

    free(processed);
    return 0;
}

void print_tuple_list(const TupleList *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->size; i++) {
        printf("(");
        for (size_t j = 0; j < list->tuples[i].size; j++) {
            printf("%d", list->tuples[i].elements[j]);
            if (j < list->tuples[i].size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }
}

int main(void) {
    TupleList input, output;
    init_tuple_list(&input);

    const int t1[] = {1, 2, 3};
    const int t2[] = {4, 5};
    const int t3[] = {1, 6, 7};
    const int t4[] = {4, 8, 9};
    const int t5[] = {2, 10};

    if (add_tuple(&input, t1, 3) != 0 ||
        add_tuple(&input, t2, 2) != 0 ||
        add_tuple(&input, t3, 3) != 0 ||
        add_tuple(&input, t4, 3) != 0 ||
        add_tuple(&input, t5, 2) != 0) {
        free_tuple_list(&input);
        return EXIT_FAILURE;
    }

    printf("Input tuples:\n");
    print_tuple_list(&input);

    if (join_tuples_by_initial(&input, &output) != 0) {
        free_tuple_list(&input);
        return EXIT_FAILURE;
    }

    printf("\nJoined tuples:\n");
    print_tuple_list(&output);

    free_tuple_list(&input);
    free_tuple_list(&output);

    return EXIT_SUCCESS;
}