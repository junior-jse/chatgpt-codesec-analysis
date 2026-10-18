#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    size_t i;
    if (lol != NULL) {
        if (lol->lists != NULL) {
            for (i = 0U; i < lol->count; i++) {
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
    dst->length = src->length;
    dst->data = NULL;
    if (src->length > 0U) {
        dst->data = (int *)malloc(src->length * sizeof(int));
        if (dst->data == NULL) {
            dst->length = 0U;
            return -1;
        }
        (void)memcpy(dst->data, src->data, src->length * sizeof(int));
    }
    return 0;
}

int find_min_length_lists(const ListOfLists *input, ResultLists *output) {
    size_t i;
    size_t min_len;
    size_t min_count;

    if (input == NULL || output == NULL) {
        return -1;
    }
    output->lists = NULL;
    output->count = 0U;

    if (input->count == 0U || input->lists == NULL) {
        return 0;
    }

    min_len = input->lists[0].length;
    for (i = 1U; i < input->count; i++) {
        if (input->lists[i].length < min_len) {
            min_len = input->lists[i].length;
        }
    }

    min_count = 0U;
    for (i = 0U; i < input->count; i++) {
        if (input->lists[i].length == min_len) {
            min_count++;
        }
    }

    output->lists = (IntList *)calloc(min_count, sizeof(IntList));
    if (output->lists == NULL) {
        return -1;
    }

    output->count = 0U;
    for (i = 0U; i < input->count; i++) {
        if (input->lists[i].length == min_len) {
            if (copy_int_list(&input->lists[i], &output->lists[output->count]) != 0) {
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
    list->length = length;
    list->data = NULL;
    if (length > 0U) {
        if (values == NULL) {
            list->length = 0U;
            return -1;
        }
        list->data = (int *)malloc(length * sizeof(int));
        if (list->data == NULL) {
            list->length = 0U;
            return -1;
        }
        (void)memcpy(list->data, values, length * sizeof(int));
    }
    return 0;
}

static void print_int_list(const IntList *list) {
    size_t j;
    if (list == NULL) {
        return;
    }
    (void)printf("[");
    for (j = 0U; j < list->length; j++) {
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
    size_t i;

    int a[] = {1, 2, 3};
    int b[] = {4, 5};
    int c[] = {6};
    int d[] = {7, 8};
    int e[] = {9};

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
    for (i = 0U; i < result.count; i++) {
        print_int_list(&result.lists[i]);
        (void)printf("\n");
    }

    for (i = 0U; i < result.count; i++) {
        free_int_list(&result.lists[i]);
    }
    free_result_lists(&result);
    free_list_of_lists(&input);

    return EXIT_SUCCESS;
}