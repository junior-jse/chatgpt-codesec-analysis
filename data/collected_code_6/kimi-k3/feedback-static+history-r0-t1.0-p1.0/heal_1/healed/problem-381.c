#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
} IntList;

typedef struct {
    IntList *lists;
    size_t count;
} ListOfLists;

static size_t g_sort_index = 0;

int compare_lists(const void *a, const void *b)
{
    const IntList *list_a = (const IntList *)a;
    const IntList *list_b = (const IntList *)b;
    int val_a = 0;
    int val_b = 0;
    
    if (g_sort_index < list_a->size) {
        val_a = list_a->data[g_sort_index];
    }
    if (g_sort_index < list_b->size) {
        val_b = list_b->data[g_sort_index];
    }
    
    if (val_a < val_b) {
        return -1;
    }
    if (val_a > val_b) {
        return 1;
    }
    return 0;
}

int sort_lists_by_index(ListOfLists *lol, size_t index)
{
    if (lol == NULL || lol->lists == NULL || lol->count == 0) {
        return -1;
    }
    
    g_sort_index = index;
    qsort(lol->lists, lol->count, sizeof(IntList), compare_lists);
    return 0;
}

void free_list_of_lists(ListOfLists *lol)
{
    if (lol == NULL) {
        return;
    }
    if (lol->lists != NULL) {
        for (size_t i = 0; i < lol->count; i++) {
            free(lol->lists[i].data);
            lol->lists[i].data = NULL;
        }
        free(lol->lists);
        lol->lists = NULL;
    }
    lol->count = 0;
}

int main(void)
{
    ListOfLists lol = {NULL, 0};
    int status;
    
    lol.count = 4;
    lol.lists = (IntList *)calloc(lol.count, sizeof(IntList));
    if (lol.lists == NULL) {
        return EXIT_FAILURE;
    }
    
    lol.lists[0].size = 3;
    lol.lists[0].data = (int *)malloc(3 * sizeof(int));
    if (lol.lists[0].data == NULL) {
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    lol.lists[0].data[0] = 5;
    lol.lists[0].data[1] = 2;
    lol.lists[0].data[2] = 8;
    
    lol.lists[1].size = 3;
    lol.lists[1].data = (int *)malloc(3 * sizeof(int));
    if (lol.lists[1].data == NULL) {
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    lol.lists[1].data[0] = 1;
    lol.lists[1].data[1] = 9;
    lol.lists[1].data[2] = 3;
    
    lol.lists[2].size = 3;
    lol.lists[2].data = (int *)malloc(3 * sizeof(int));
    if (lol.lists[2].data == NULL) {
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    lol.lists[2].data[0] = 4;
    lol.lists[2].data[1] = 1;
    lol.lists[2].data[2] = 7;
    
    lol.lists[3].size = 2;
    lol.lists[3].data = (int *)malloc(2 * sizeof(int));
    if (lol.lists[3].data == NULL) {
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    lol.lists[3].data[0] = 2;
    lol.lists[3].data[1] = 6;
    
    status = sort_lists_by_index(&lol, 1);
    if (status != 0) {
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < lol.count; i++) {
        for (size_t j = 0; j < lol.lists[i].size; j++) {
            printf("%d ", lol.lists[i].data[j]);
        }
        printf("\n");
    }
    
    free_list_of_lists(&lol);
    return EXIT_SUCCESS;
}