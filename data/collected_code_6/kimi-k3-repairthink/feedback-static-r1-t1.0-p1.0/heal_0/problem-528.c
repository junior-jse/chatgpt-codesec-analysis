#include <stdio.h>
#include <stdlib.h>

typedef struct IntList {
    int *data;
    size_t length;
} IntList;

typedef struct ListOfLists {
    IntList *lists;
    size_t count;
} ListOfLists;

static void init_int_list(IntList *list)
{
    if (list != NULL) {
        list->data = NULL;
        list->length = 0U;
    }
}

static void init_list_of_lists(ListOfLists *container)
{
    if (container != NULL) {
        container->lists = NULL;
        container->count = 0U;
    }
}

static int add_int_to_list(IntList *list, int value)
{
    int *new_data;
    size_t new_length;

    if (list == NULL) {
        return 0;
    }

    new_length = list->length + 1U;
    if (new_length < list->length) {
        return 0;
    }

    new_data = (int *)realloc(list->data, new_length * sizeof(int));
    if (new_data == NULL) {
        return 0;
    }

    list->data = new_data;
    list->data[list->length] = value;
    list->length = new_length;

    return 1;
}

static int add_list_to_container(ListOfLists *container, const IntList *list)
{
    IntList *new_lists;
    size_t new_count;

    if ((container == NULL) || (list == NULL)) {
        return 0;
    }

    new_count = container->count + 1U;
    if (new_count < container->count) {
        return 0;
    }

    new_lists = (IntList *)realloc(container->lists, new_count * sizeof(IntList));
    if (new_lists == NULL) {
        return 0;
    }

    container->lists = new_lists;
    container->lists[container->count] = *list;
    container->count = new_count;

    return 1;
}

static void free_int_list(IntList *list)
{
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->length = 0U;
    }
}

static void free_list_of_lists(ListOfLists *container)
{
    size_t i;

    if (container != NULL) {
        if (container->lists != NULL) {
            for (i = 0U; i < container->count; ++i) {
                free_int_list(&container->lists[i]);
            }
            free(container->lists);
        }
        container->lists = NULL;
        container->count = 0U;
    }
}

static size_t find_min_length(const ListOfLists *container, int *found)
{
    size_t min_length;
    size_t i;

    if (found != NULL) {
        *found = 0;
    }

    if ((container == NULL) || (container->lists == NULL) || (container->count == 0U)) {
        return 0U;
    }

    min_length = container->lists[0].length;
    for (i = 1U; i < container->count; ++i) {
        if (container->lists[i].length < min_length) {
            min_length = container->lists[i].length;
        }
    }

    if (found != NULL) {
        *found = 1;
    }

    return min_length;
}

static size_t collect_min_length_lists(const ListOfLists *container, size_t min_length, size_t **indices_out)
{
    size_t *indices;
    size_t match_count;
    size_t i;

    if (indices_out == NULL) {
        return 0U;
    }

    *indices_out = NULL;

    if ((container == NULL) || (container->lists == NULL) || (container->count == 0U)) {
        return 0U;
    }

    indices = (size_t *)malloc(container->count * sizeof(size_t));
    if (indices == NULL) {
        return 0U;
    }

    match_count = 0U;
    for (i = 0U; i < container->count; ++i) {
        if (container->lists[i].length == min_length) {
            indices[match_count] = i;
            ++match_count;
        }
    }

    if (match_count == 0U) {
        free(indices);
        return 0U;
    }

    *indices_out = indices;
    return match_count;
}

static void print_int_list(const IntList *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    printf("[");
    for (i = 0U; i < list->length; ++i) {
        if (i > 0U) {
            printf(", ");
        }
        printf("%d", list->data[i]);
    }
    printf("]");
}

int main(void)
{
    ListOfLists container;
    IntList list1;
    IntList list2;
    IntList list3;
    IntList list4;
    size_t *min_indices;
    size_t min_count;
    size_t min_length;
    size_t i;
    int found;
    int success;

    init_list_of_lists(&container);
    init_int_list(&list1);
    init_int_list(&list2);
    init_int_list(&list3);
    init_int_list(&list4);

    min_indices = NULL;
    success = 1;

    success = success && add_int_to_list(&list1, 1);
    success = success && add_int_to_list(&list1, 2);
    success = success && add_int_to_list(&list1, 3);

    success = success && add_int_to_list(&list2, 4);
    success = success && add_int_to_list(&list2, 5);

    success = success && add_int_to_list(&list3, 6);
    success = success && add_int_to_list(&list3, 7);

    success = success && add_int_to_list(&list4, 8);
    success = success && add_int_to_list(&list4, 9);
    success = success && add_int_to_list(&list4, 10);
    success = success && add_int_to_list(&list4, 11);

    if (success) {
        success = add_list_to_container(&container, &list1);
    }
    if (success) {
        init_int_list(&list1);
        success = add_list_to_container(&container, &list2);
    }
    if (success) {
        init_int_list(&list2);
        success = add_list_to_container(&container, &list3);
    }
    if (success) {
        init_int_list(&list3);
        success = add_list_to_container(&container, &list4);
    }
    if (success) {
        init_int_list(&list4);
    }

    if (success) {
        min_length = find_min_length(&container, &found);
        if (found) {
            min_count = collect_min_length_lists(&container, min_length, &min_indices);
            if ((min_count > 0U) && (min_indices != NULL)) {
                printf("Minimum length: %zu\n", min_length);
                printf("Lists with minimum length:\n");
                for (i = 0U; i < min_count; ++i) {
                    printf("  List %zu: ", min_indices[i]);
                    print_int_list(&container.lists[min_indices[i]]);
                    printf("\n");
                }
            } else {
                printf("Failed to collect minimum length lists.\n");
            }
        } else {
            printf("The container is empty.\n");
        }
    } else {
        printf("Failed to build the list of lists.\n");
    }

    free(min_indices);
    free_list_of_lists(&container);
    free_int_list(&list1);
    free_int_list(&list2);
    free_int_list(&list3);
    free_int_list(&list4);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}