#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int *elements;
    size_t count;
} IntList;

static bool contains(const int *list, size_t count, int value)
{
    if (list == NULL) {
        return false;
    }

    for (size_t i = 0; i < count; ++i) {
        if (list[i] == value) {
            return true;
        }
    }

    return false;
}

static IntList *find_nested_elements(
    const IntList *nested_lists,
    size_t nested_count,
    const int *other_list,
    size_t other_count)
{
    if ((nested_count > 0U && nested_lists == NULL) ||
        (other_count > 0U && other_list == NULL)) {
        return NULL;
    }

    IntList *result = calloc(nested_count, sizeof(*result));
    if (nested_count > 0U && result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < nested_count; ++i) {
        if (nested_lists[i].count > 0U && nested_lists[i].elements == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(result[j].elements);
            }
            free(result);
            return NULL;
        }

        if (nested_lists[i].count == 0U) {
            continue;
        }

        result[i].elements = malloc(
            nested_lists[i].count * sizeof(*result[i].elements));
        if (result[i].elements == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(result[j].elements);
            }
            free(result);
            return NULL;
        }

        for (size_t j = 0; j < nested_lists[i].count; ++j) {
            int value = nested_lists[i].elements[j];
            if (contains(other_list, other_count, value)) {
                result[i].elements[result[i].count++] = value;
            }
        }

        if (result[i].count == 0U) {
            free(result[i].elements);
            result[i].elements = NULL;
        } else {
            int *resized = realloc(
                result[i].elements,
                result[i].count * sizeof(*result[i].elements));
            if (resized != NULL) {
                result[i].elements = resized;
            }
        }
    }

    return result;
}

static void free_nested_lists(IntList *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(lists[i].elements);
    }

    free(lists);
}

static void print_nested_lists(const IntList *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    printf("[");
    for (size_t i = 0; i < count; ++i) {
        printf("[");
        for (size_t j = 0; j < lists[i].count; ++j) {
            printf("%d", lists[i].elements[j]);
            if (j + 1U < lists[i].count) {
                printf(", ");
            }
        }
        printf("]");
        if (i + 1U < count) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void)
{
    int first[] = {1, 2, 3};
    int second[] = {4, 5, 6};
    int third[] = {2, 6, 8};
    int other[] = {2, 4, 6, 10};

    IntList nested[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };

    size_t nested_count = sizeof(nested) / sizeof(nested[0]);
    size_t other_count = sizeof(other) / sizeof(other[0]);

    IntList *result = find_nested_elements(
        nested, nested_count, other, other_count);
    if (result == NULL) {
        fputs("Failed to find matching elements.\n", stderr);
        return EXIT_FAILURE;
    }

    print_nested_lists(result, nested_count);
    free_nested_lists(result, nested_count);

    return EXIT_SUCCESS;
}