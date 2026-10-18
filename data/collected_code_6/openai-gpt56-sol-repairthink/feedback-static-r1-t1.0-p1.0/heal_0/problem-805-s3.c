#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

typedef struct {
    int *elements;
    size_t length;
} IntList;

static int sum_list(const IntList *list, int64_t *sum)
{
    int64_t total = 0;

    if (list == NULL || sum == NULL ||
        (list->length > 0 && list->elements == NULL)) {
        return -1;
    }

    for (size_t i = 0; i < list->length; ++i) {
        total += list->elements[i];
    }

    *sum = total;
    return 0;
}

static int find_highest_sum_list(const IntList *lists, size_t count,
                                 size_t *best_index, int64_t *best_sum)
{
    int64_t current_sum;

    if (lists == NULL || count == 0 || best_index == NULL || best_sum == NULL) {
        return -1;
    }

    if (sum_list(&lists[0], best_sum) != 0) {
        return -1;
    }

    *best_index = 0;

    for (size_t i = 1; i < count; ++i) {
        if (sum_list(&lists[i], &current_sum) != 0) {
            return -1;
        }

        if (current_sum > *best_sum) {
            *best_sum = current_sum;
            *best_index = i;
        }
    }

    return 0;
}

static int read_size(const char *prompt, size_t *value)
{
    unsigned long long input;

    if (prompt == NULL || value == NULL) {
        return -1;
    }

    if (printf("%s", prompt) < 0 || fflush(stdout) != 0) {
        return -1;
    }

    errno = 0;
    if (scanf("%llu", &input) != 1 || errno == ERANGE || input > SIZE_MAX) {
        return -1;
    }

    *value = (size_t)input;
    return 0;
}

static void free_lists(IntList *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(lists[i].elements);
    }

    free(lists);
}

int main(void)
{
    IntList *lists = NULL;
    size_t list_count;
    size_t best_index;
    int64_t best_sum;
    int exit_status = EXIT_FAILURE;

    if (read_size("Number of lists: ", &list_count) != 0 || list_count == 0 ||
        list_count > SIZE_MAX / sizeof(*lists)) {
        fputs("Invalid number of lists.\n", stderr);
        return EXIT_FAILURE;
    }

    lists = calloc(list_count, sizeof(*lists));
    if (lists == NULL) {
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list_count; ++i) {
        if (printf("Length of list %zu: ", i) < 0 || fflush(stdout) != 0 ||
            scanf("%zu", &lists[i].length) != 1 ||
            lists[i].length > SIZE_MAX / sizeof(*lists[i].elements)) {
            fputs("Invalid list length.\n", stderr);
            goto cleanup;
        }

        if (lists[i].length > 0) {
            lists[i].elements = malloc(lists[i].length *
                                       sizeof(*lists[i].elements));
            if (lists[i].elements == NULL) {
                fputs("Memory allocation failed.\n", stderr);
                goto cleanup;
            }
        }

        if (printf("Elements of list %zu: ", i) < 0 || fflush(stdout) != 0) {
            fputs("Output error.\n", stderr);
            goto cleanup;
        }

        for (size_t j = 0; j < lists[i].length; ++j) {
            if (scanf("%d", &lists[i].elements[j]) != 1) {
                fputs("Invalid element.\n", stderr);
                goto cleanup;
            }
        }
    }

    if (find_highest_sum_list(lists, list_count, &best_index, &best_sum) != 0) {
        fputs("Unable to find the highest-sum list.\n", stderr);
        goto cleanup;
    }

    if (printf("List with highest sum: [") < 0) {
        goto cleanup;
    }

    for (size_t i = 0; i < lists[best_index].length; ++i) {
        if (printf("%s%d", i == 0 ? "" : ", ",
                   lists[best_index].elements[i]) < 0) {
            goto cleanup;
        }
    }

    if (printf("]\nSum: %" PRId64 "\n", best_sum) < 0) {
        goto cleanup;
    }

    exit_status = EXIT_SUCCESS;

cleanup:
    free_lists(lists, list_count);
    return exit_status;
}