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

typedef struct {
    int first;
    int second;
} Pair;

typedef struct {
    Pair *pairs;
    size_t count;
} ZippedList;

ZippedList zip_lists(const IntList *list1, const IntList *list2) {
    ZippedList result;
    result.count = 0;
    result.pairs = NULL;

    if (list1 == NULL || list2 == NULL) {
        return result;
    }

    size_t min_size = (list1->size < list2->size) ? list1->size : list2->size;

    if (min_size == 0) {
        return result;
    }

    result.pairs = (Pair *)malloc(min_size * sizeof(Pair));
    if (result.pairs == NULL) {
        return result;
    }

    result.count = min_size;

    for (size_t i = 0; i < min_size; i++) {
        result.pairs[i].first = list1->data[i];
        result.pairs[i].second = list2->data[i];
    }

    return result;
}

void free_zipped_list(ZippedList *zipped) {
    if (zipped != NULL && zipped->pairs != NULL) {
        free(zipped->pairs);
        zipped->pairs = NULL;
        zipped->count = 0;
    }
}

int main(void) {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {10, 20, 30, 40, 50};

    IntList list1;
    list1.data = data1;
    list1.size = sizeof(data1) / sizeof(data1[0]);

    IntList list2;
    list2.data = data2;
    list2.size = sizeof(data2) / sizeof(data2[0]);

    ZippedList zipped = zip_lists(&list1, &list2);

    if (zipped.pairs != NULL) {
        for (size_t i = 0; i < zipped.count; i++) {
            printf("(%d, %d)\n", zipped.pairs[i].first, zipped.pairs[i].second);
        }
        free_zipped_list(&zipped);
    }

    return 0;
}