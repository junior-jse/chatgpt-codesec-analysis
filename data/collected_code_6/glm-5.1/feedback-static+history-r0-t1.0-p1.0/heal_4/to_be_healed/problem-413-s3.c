#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
    int c;
} Tuple;

typedef struct {
    Tuple *data;
    size_t length;
} TupleList;

int extract_nth_element(const TupleList *list, size_t n, size_t element_index, int *out_value) {
    if (list == NULL || out_value == NULL) {
        return -1;
    }

    if (list->data == NULL) {
        return -1;
    }

    if (n >= list->length) {
        return -1;
    }

    if (element_index >= 3) {
        return -1;
    }

    if (element_index == 0) {
        *out_value = list->data[n].a;
    } else if (element_index == 1) {
        *out_value = list->data[n].b;
    } else {
        *out_value = list->data[n].c;
    }

    return 0;
}

int main(void) {
    Tuple array[] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    TupleList list = {array, 3};

    int value;
    int ret;

    ret = extract_nth_element(&list, 1, 2, &value);
    if (ret == 0) {
        printf("%d\n", value);
    }

    /* Possible weaknesses found:
     *  Calling function 'extract_nth_element' returns -1
     *  Assignment 'ret=extract_nth_element(NULL,0,0,&value)', assigned value is -1
     */
    ret = extract_nth_element(NULL, 0, 0, &value);
    /* Possible weaknesses found:
     *  Condition 'ret!=0' is always true [knownConditionTrueFalse]
     *  Condition 'ret!=0' is always true
     */
    if (ret != 0) {
        printf("FAIL\n");
    }

    ret = extract_nth_element(&list, 5, 0, &value);
    if (ret != 0) {
        printf("FAIL\n");
    }

    ret = extract_nth_element(&list, 0, 5, &value);
    if (ret != 0) {
        printf("FAIL\n");
    }

    return 0;
}