#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Tuple {
    int *elements;
    int size;
};

struct TupleList {
    struct Tuple *tuples;
    int count;
};

bool check_all_k_elements(struct TupleList *list, int k) {
    if (list == NULL) {
        return false;
    }
    if (list->tuples == NULL && list->count > 0) {
        return false;
    }
    for (int i = 0; i < list->count; i++) {
        if (list->tuples[i].elements == NULL) {
            return false;
        }
        if (list->tuples[i].size != k) {
            return false;
        }
    }
    return true;
}

int main(void) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5, 6};
    struct Tuple t1 = {data1, 3};
    struct Tuple t2 = {data2, 3};
    struct Tuple list1_tuples[] = {t1, t2};
    struct TupleList list1 = {list1_tuples, 2};

    int data3[] = {7, 8};
    int data4[] = {9, 10, 11};
    struct Tuple t3 = {data3, 2};
    struct Tuple t4 = {data4, 3};
    struct Tuple list2_tuples[] = {t3, t4};
    struct TupleList list2 = {list2_tuples, 2};

    bool res1 = check_all_k_elements(&list1, 3);
    bool res2 = check_all_k_elements(&list2, 3);
    bool res3 = check_all_k_elements(NULL, 3);

    printf("List 1 all 3-elements: %s\n", res1 ? "true" : "false");
    printf("List 2 all 3-elements: %s\n", res2 ? "true" : "false");
    printf("NULL list all 3-elements: %s\n", res3 ? "true" : "false");

    return 0;
}