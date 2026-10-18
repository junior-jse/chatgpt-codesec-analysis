#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

typedef struct IntList {
    int *data;
    size_t length;
} IntList;

typedef struct ListOfLists {
    IntList *lists;
    size_t count;
} ListOfLists;

typedef struct ResultLists {
    IntList *lists;
    size_t count;
} ResultLists;

static void free_int_list(IntList *list) {
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->length = 0U;
    }
}

static void free_list_of_lists(ListOfLists *lol) {
    if (lol != NULL) {
        if (lol->lists != NULL) {
            for (size_t i = 0U; i < lol->count; i++) {
                free_int_list(&lol->lists[i]);
            }
            free(lol->lists);
            lol->lists = NULL;
        }
        lol->count = 0U;
    }
}

static void free_result_lists(ResultLists *result) {
    if (result != NULL) {
        free(result->lists);
        result->lists = NULL;
        result->count = 0U;
    }
}

static int copy_int_list(const IntList *src, IntList *dst) {
    if (src == NULL || dst == NULL) {
        return -1;
    }
    dst->length = 0U;
    dst->data = NULL;
    if (src->length > 0U) {
        if (src->length > SIZE_MAX / sizeof(int)) {
            return -1;
        }
        if (src->data == NULL) {
            return -1;
        }
        size_t copy_size = src->length * sizeof(int);
        dst->data = (int *)malloc(copy_size);
        if (dst->data == NULL) {
            return -1;
        }
        dst->length = src->length;
        /* Possible weaknesses found:
         *  Condition 'src->data!=NULL' is always true [knownConditionTrueFalse]
         */
        if (copy_size > 0U && dst->data != NULL && src->data != NULL) {
            /* Possible weaknesses found:
             *  implicit declaration of function 'memcpy_s'; did you mean 'memcpy'? [-Wimplicit-function-declaration]
             *  did you mean 'memcpy'?
             *  call to undeclared function 'memcpy_s'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
             */
            memcpy_s(dst->data, copy_size, src->data, copy_size);
        }
    }
    return 0;
}

int find_min_length_lists(const ListOfLists *input, ResultLists *output) {
    /* Possible weaknesses found:
     *  'memcpy' declared here
     */
    if (input == NULL || output == NULL) {
        return -1;
    }
    output->lists = NULL;
    output->count = 0U;

    if (input->count == 0U || input->lists == NULL) {
        return 0;
    }

    size_t min_len = input->lists[0].length;
    for (size_t i = 1U; i < input->count; i++) {
        if (input->lists[i].length < min_len) {
            min_len = input->lists[i].length;
        }
    }

    size_t min_count = 0U;
    for (size_t i = 0U; i < input->count; i++) {
        if (input->lists[i].length == min_len) {
            min_count++;
        }
    }

    if (min_count > SIZE_MAX / sizeof(IntList)) {
        return -1;
    }

    output->lists = (IntList *)calloc(min_count, sizeof(IntList));
    if (output->lists == NULL) {
        return -1;
    }

    output->count = 0U;
    for (size_t i = 0U; i < input->count; i++) {
        if (input->lists[i].length == min_len) {
            if (output->count >= min_count) {
                for (size_t j = 0U; j < output->count; j++) {
                    free_int_list(&output->lists[j]);
                }
                free_result_lists(output);
                return -1;
            }
            if (copy_int_list(&input->lists[i], &output->lists[output->count]) != 0) {
                for (size_t j = 0U; j < output->count; j++) {
                    free_int_list(&output->lists[j]);
                }
                free_result_lists(output);
                return -1;
            }
            output->count++;
        }
    }

    return 0;
}

static int init_int_list(IntList *list, const int *values, size_t length) {
    if (list == NULL) {
        return -1;
    }
    list->length = 0U;
    list->data = NULL;
    if (length > 0U) {
        if (values == NULL) {
            return -1;
        }
        if (length > SIZE_MAX / sizeof(int)) {
            return -1;
        }
        size_t copy_size = length * sizeof(int);
        list->data = (int *)malloc(copy_size);
        if (list->data == NULL) {
            return -1;
        }
        list->length = length;
        if (copy_size > 0U && list->data != NULL && values != NULL) {
            /* Possible weaknesses found:
             *  call to undeclared function 'memcpy_s'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
             */
            memcpy_s(list->data, copy_size, values, copy_size);
        }
    }
    return 0;
}

static void print_int_list(const IntList *list) {
    if (list == NULL) {
        return;
    }
    (void)printf("[");
    for (size_t j = 0U; j < list->length; j++) {
        (void)printf("%d", list->data[j]);
        if (j + 1U < list->length) {
            (void)printf(", ");
        }
    }
    (void)printf("]");
}

int main(void) {
    ListOfLists input;
    ResultLists result;
    int status;

    const int a[] = {1, 2, 3};
    const int b[] = {4, 5};
    const int c[] = {6};
    const int d[] = {7, 8};
    const int e[] = {9};

    input.lists = (IntList *)calloc(5U, sizeof(IntList));
    if (input.lists == NULL) {
        (void)fprintf(stderr, "Allocation failed\n");
        return EXIT_FAILURE;
    }
    input.count = 5U;

    if (init_int_list(&input.lists[0], a, 3U) != 0 ||
        init_int_list(&input.lists[1], b, 2U) != 0 ||
        init_int_list(&input.lists[2], c, 1U) != 0 ||
        init_int_list(&input.lists[3], d, 2U) != 0 ||
        init_int_list(&input.lists[4], e, 1U) != 0) {
        (void)fprintf(stderr, "Initialization failed\n");
        free_list_of_lists(&input);
        return EXIT_FAILURE;
    }

    result.lists = NULL;
    result.count = 0U;

    status = find_min_length_lists(&input, &result);
    if (status != 0) {
        (void)fprintf(stderr, "find_min_length_lists failed\n");
        free_list_of_lists(&input);
        return EXIT_FAILURE;
    }

    (void)printf("Lists with minimum length (%zu):\n", result.count > 0U ? result.lists[0].length : 0U);
    for (size_t i = 0U; i < result.count; i++) {
        print_int_list(&result.lists[i]);
        (void)printf("\n");
    }

    for (size_t i = 0U; i < result.count; i++) {
        free_int_list(&result.lists[i]);
    }
    free_result_lists(&result);
    free_list_of_lists(&input);

    return EXIT_SUCCESS;
}