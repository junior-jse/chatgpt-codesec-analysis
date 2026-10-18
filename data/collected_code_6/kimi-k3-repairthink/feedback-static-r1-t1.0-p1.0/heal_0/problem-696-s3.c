#include <stdio.h>
#include <stdlib.h>

typedef struct IntList {
    int *data;
    size_t size;
} IntList;

typedef struct ListOfLists {
    IntList *lists;
    size_t count;
} ListOfLists;

typedef struct ZippedPair {
    int first;
    int second;
} ZippedPair;

typedef struct ZippedList {
    ZippedPair *pairs;
    size_t size;
} ZippedList;

typedef struct ZippedResult {
    ZippedList *lists;
    size_t count;
} ZippedResult;

void init_int_list(IntList *list, size_t size) {
    list->size = size;
    list->data = malloc(size * sizeof(int));
    if (list->data == NULL) {
        list->size = 0;
    }
}

void free_int_list(IntList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
}

void init_list_of_lists(ListOfLists *lol, size_t count) {
    lol->count = count;
    lol->lists = malloc(count * sizeof(IntList));
    if (lol->lists == NULL) {
        lol->count = 0;
    }
}

void free_list_of_lists(ListOfLists *lol) {
    size_t i;
    for (i = 0; i < lol->count; i++) {
        free_int_list(&lol->lists[i]);
    }
    free(lol->lists);
    lol->lists = NULL;
    lol->count = 0;
}

void free_zipped_result(ZippedResult *result) {
    size_t i;
    for (i = 0; i < result->count; i++) {
        free(result->lists[i].pairs);
    }
    free(result->lists);
    result->lists = NULL;
    result->count = 0;
}

int zip_lists_of_lists(const ListOfLists *a, const ListOfLists *b, ZippedResult *result) {
    size_t i, j;
    size_t min_count;
    
    if (a == NULL || b == NULL || result == NULL) {
        return -1;
    }
    
    if (a->count == 0 || b->count == 0) {
        result->lists = NULL;
        result->count = 0;
        return 0;
    }
    
    min_count = a->count < b->count ? a->count : b->count;
    
    result->lists = malloc(min_count * sizeof(ZippedList));
    if (result->lists == NULL) {
        return -1;
    }
    result->count = min_count;
    
    for (i = 0; i < min_count; i++) {
        size_t min_size = a->lists[i].size < b->lists[i].size ? 
                          a->lists[i].size : b->lists[i].size;
        
        result->lists[i].pairs = malloc(min_size * sizeof(ZippedPair));
        if (result->lists[i].pairs == NULL) {
            size_t k;
            for (k = 0; k < i; k++) {
                free(result->lists[k].pairs);
            }
            free(result->lists);
            result->lists = NULL;
            result->count = 0;
            return -1;
        }
        result->lists[i].size = min_size;
        
        for (j = 0; j < min_size; j++) {
            result->lists[i].pairs[j].first = a->lists[i].data[j];
            result->lists[i].pairs[j].second = b->lists[i].data[j];
        }
    }
    
    return 0;
}

void print_zipped_result(const ZippedResult *result) {
    size_t i, j;
    
    if (result == NULL) {
        return;
    }
    
    printf("[");
    for (i = 0; i < result->count; i++) {
        printf("[");
        for (j = 0; j < result->lists[i].size; j++) {
            printf("(%d, %d)", result->lists[i].pairs[j].first, 
                   result->lists[i].pairs[j].second);
            if (j < result->lists[i].size - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i < result->count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    ListOfLists list_a;
    ListOfLists list_b;
    ZippedResult result;
    int status;
    size_t i, j;
    
    init_list_of_lists(&list_a, 3);
    if (list_a.lists == NULL) {
        return EXIT_FAILURE;
    }
    
    init_int_list(&list_a.lists[0], 3);
    init_int_list(&list_a.lists[1], 2);
    init_int_list(&list_a.lists[2], 4);
    
    for (i = 0; i < list_a.count; i++) {
        if (list_a.lists[i].data == NULL) {
            for (j = 0; j < i; j++) {
                free_int_list(&list_a.lists[j]);
            }
            free(list_a.lists);
            return EXIT_FAILURE;
        }
    }
    
    list_a.lists[0].data[0] = 1;
    list_a.lists[0].data[1] = 2;
    list_a.lists[0].data[2] = 3;
    list_a.lists[1].data[0] = 4;
    list_a.lists[1].data[1] = 5;
    list_a.lists[2].data[0] = 6;
    list_a.lists[2].data[1] = 7;
    list_a.lists[2].data[2] = 8;
    list_a.lists[2].data[3] = 9;
    
    init_list_of_lists(&list_b, 3);
    if (list_b.lists == NULL) {
        free_list_of_lists(&list_a);
        return EXIT_FAILURE;
    }
    
    init_int_list(&list_b.lists[0], 3);
    init_int_list(&list_b.lists[1], 2);
    init_int_list(&list_b.lists[2], 4);
    
    for (i = 0; i < list_b.count; i++) {
        if (list_b.lists[i].data == NULL) {
            for (j = 0; j < i; j++) {
                free_int_list(&list_b.lists[j]);
            }
            free(list_b.lists);
            free_list_of_lists(&list_a);
            return EXIT_FAILURE;
        }
    }
    
    list_b.lists[0].data[0] = 10;
    list_b.lists[0].data[1] = 20;
    list_b.lists[0].data[2] = 30;
    list_b.lists[1].data[0] = 40;
    list_b.lists[1].data[1] = 50;
    list_b.lists[2].data[0] = 60;
    list_b.lists[2].data[1] = 70;
    list_b.lists[2].data[2] = 80;
    list_b.lists[2].data[3] = 90;
    
    status = zip_lists_of_lists(&list_a, &list_b, &result);
    if (status != 0) {
        free_list_of_lists(&list_a);
        free_list_of_lists(&list_b);
        return EXIT_FAILURE;
    }
    
    printf("List A: [[1, 2, 3], [4, 5], [6, 7, 8, 9]]\n");
    printf("List B: [[10, 20, 30], [40, 50], [60, 70, 80, 90]]\n");
    printf("Zipped: ");
    print_zipped_result(&result);
    
    free_zipped_result(&result);
    free_list_of_lists(&list_a);
    free_list_of_lists(&list_b);
    
    return EXIT_SUCCESS;
}