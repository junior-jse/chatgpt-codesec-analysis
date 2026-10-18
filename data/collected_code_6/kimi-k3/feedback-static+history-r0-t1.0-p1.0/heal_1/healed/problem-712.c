#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} IntList;

typedef struct {
    IntList *lists;
    size_t count;
    size_t capacity;
} ListOfLists;

static int compare_ints(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

static int lists_equal(const IntList *a, const IntList *b)
{
    if (a->size != b->size) return 0;
    if (a->size == 0) return 1;
    return memcmp(a->data, b->data, a->size * sizeof(int)) == 0;
}

static int list_contains(const ListOfLists *lol, const IntList *list)
{
    size_t i;
    for (i = 0; i < lol->count; ++i) {
        if (lists_equal(&lol->lists[i], list)) return 1;
    }
    return 0;
}

static int append_list(ListOfLists *lol, const IntList *list)
{
    IntList *new_lists;
    int *new_data;

    if (lol->count == lol->capacity) {
        size_t new_capacity = lol->capacity == 0 ? 4 : lol->capacity * 2;
        new_lists = realloc(lol->lists, new_capacity * sizeof(*new_lists));
        if (new_lists == NULL) return 0;
        lol->lists = new_lists;
        lol->capacity = new_capacity;
    }

    new_data = malloc(list->size * sizeof(*new_data));
    if (new_data == NULL && list->size > 0) return 0;

    if (list->size > 0) {
        memcpy(new_data, list->data, list->size * sizeof(*new_data));
    }

    lol->lists[lol->count].data = new_data;
    lol->lists[lol->count].size = list->size;
    lol->count++;
    return 1;
}

static void free_list_of_lists(ListOfLists *lol)
{
    size_t i;
    if (lol == NULL) return;
    for (i = 0; i < lol->count; ++i) {
        free(lol->lists[i].data);
    }
    free(lol->lists);
    lol->lists = NULL;
    lol->count = 0;
    lol->capacity = 0;
}

static int remove_duplicates(ListOfLists *lol)
{
    ListOfLists unique = {NULL, 0, 0};
    size_t i;

    if (lol == NULL) return 0;

    for (i = 0; i < lol->count; ++i) {
        if (!list_contains(&unique, &lol->lists[i])) {
            if (!append_list(&unique, &lol->lists[i])) {
                free_list_of_lists(&unique);
                return 0;
            }
        }
    }

    free_list_of_lists(lol);
    *lol = unique;
    return 1;
}

static int remove_duplicates_within_lists(ListOfLists *lol)
{
    size_t i, j, write_idx;

    if (lol == NULL) return 0;

    for (i = 0; i < lol->count; ++i) {
        IntList *list = &lol->lists[i];
        if (list->size == 0) continue;

        qsort(list->data, list->size, sizeof(int), compare_ints);

        write_idx = 1;
        for (j = 1; j < list->size; ++j) {
            if (list->data[j] != list->data[j - 1]) {
                list->data[write_idx++] = list->data[j];
            }
        }
        list->size = write_idx;
    }
    return 1;
}

static void print_list_of_lists(const ListOfLists *lol)
{
    size_t i, j;
    if (lol == NULL) return;

    printf("[\n");
    for (i = 0; i < lol->count; ++i) {
        printf("  [");
        for (j = 0; j < lol->lists[i].size; ++j) {
            printf("%d%s", lol->lists[i].data[j],
                   j < lol->lists[i].size - 1 ? ", " : "");
        }
        printf("]\n");
    }
    printf("]\n");
}

static int create_and_add_list(ListOfLists *lol, const int *data, size_t size)
{
    IntList temp;
    temp.data = (int *)data;
    temp.size = size;
    return append_list(lol, &temp);
}

int main(void)
{
    ListOfLists lol = {NULL, 0, 0};
    int status = 1;

    const int a[] = {1, 2, 2, 3};
    const int b[] = {4, 5};
    const int c[] = {1, 2, 3};
    const int d[] = {4, 5};
    const int e[] = {6, 6, 6};

    status = status && create_and_add_list(&lol, a, 4);
    status = status && create_and_add_list(&lol, b, 2);
    status = status && create_and_add_list(&lol, c, 3);
    status = status && create_and_add_list(&lol, d, 2);
    status = status && create_and_add_list(&lol, e, 3);

    if (!status) {
        free_list_of_lists(&lol);
        fprintf(stderr, "Failed to build list\n");
        return EXIT_FAILURE;
    }

    printf("Original:\n");
    print_list_of_lists(&lol);

    if (!remove_duplicates_within_lists(&lol)) {
        free_list_of_lists(&lol);
        fprintf(stderr, "Failed to remove duplicates within lists\n");
        return EXIT_FAILURE;
    }

    printf("After removing duplicates within lists:\n");
    print_list_of_lists(&lol);

    if (!remove_duplicates(&lol)) {
        free_list_of_lists(&lol);
        fprintf(stderr, "Failed to remove duplicate lists\n");
        return EXIT_FAILURE;
    }

    printf("After removing duplicate lists:\n");
    print_list_of_lists(&lol);

    free_list_of_lists(&lol);
    return EXIT_SUCCESS;
}