#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *pairs;
    int capacity;
    int size;
} Dictionary;

typedef struct {
    Dictionary *dicts;
    int count;
} DictList;

int all_empty(DictList list) {
    if (list.count == 0) {
        return 1;
    }
    for (int i = 0; i < list.count; i++) {
        if (list.dicts[i].size > 0) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Dictionary d1 = {NULL, 0, 0};
    Dictionary d2 = {(int *)malloc(10 * sizeof(int)), 10, 0};
    if (d2.pairs == NULL) {
        free(d1.pairs);
        return 1;
    }

    DictList list1 = {(Dictionary []){d1, d2}, 2};

    int res1 = all_empty(list1);
    printf("%d\n", res1);

    d2.size = 5;
    DictList list2 = {(Dictionary []){d1, d2}, 2};

    int res2 = all_empty(list2);
    printf("%d\n", res2);

    free(d1.pairs);
    free(d2.pairs);
    d1.pairs = NULL;
    d2.pairs = NULL;

    return 0;
}