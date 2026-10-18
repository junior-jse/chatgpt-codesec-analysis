#include <stdio.h>
#include <stdlib.h>

struct Tuple {
    int *elements;
    int size;
};

struct TupleList {
    struct Tuple *tuples;
    int count;
};

int check_all_k_elements(const struct TupleList *list, int k) {
    if (list == NULL) {
        return -1;
    }
    if (list->tuples == NULL && list->count > 0) {
        return -1;
    }
    for (int i = 0; i < list->count; i++) {
        if (list->tuples[i].elements == NULL) {
            return -1;
        }
        if (list->tuples[i].size != k) {
            return 0;
        }
    }
    return 1;
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

    int res1 = check_all_k_elements(&list1, 3);
    int res2 = check_all_k_elements(&list2, 3);
    /* Possible weaknesses found:
     *  Calling function 'check_all_k_elements' returns -1
     *  Assignment 'res3=check_all_k_elements(NULL,3)', assigned value is -1
     */
    int res3 = check_all_k_elements(NULL, 3);

    printf("List 1 all 3-elements: %s\n", res1 == 1 ? "true" : "false");
    printf("List 2 all 3-elements: %s\n", res2 == 1 ? "true" : "false");
    /* Possible weaknesses found:
     *  Condition 'res3==-1' is always true [knownConditionTrueFalse]
     *  Condition 'res3==-1' is always true
     */
    printf("NULL list all 3-elements: %s\n", res3 == -1 ? "error" : (res3 == 1 ? "true" : "false"));

    return 0;
}