#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
    int c;
} Tuple;

int extract_nth_element(Tuple *list, size_t list_size, size_t n, size_t index, int *result) {
    if (list == NULL || result == NULL) {
        return -1;
    }

    if (index >= list_size) {
        return -2;
    }

    switch (n) {
        case 0:
            *result = list[index].a;
            break;
        case 1:
            *result = list[index].b;
            break;
        case 2:
            *result = list[index].c;
            break;
        default:
            return -3;
    }

    return 0;
}

int main(void) {
    Tuple list[] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    size_t list_size = sizeof(list) / sizeof(list[0]);
    int result;
    int ret;

    ret = extract_nth_element(list, list_size, 1, 2, &result);
    if (ret == 0) {
        printf("%d\n", result);
    } else {
        printf("Error: %d\n", ret);
    }

    /* Possible weaknesses found:
     *  Calling function 'extract_nth_element' returns -1
     *  Assignment 'ret=extract_nth_element(NULL,list_size,0,0,&result)', assigned value is -1
     */
    ret = extract_nth_element(NULL, list_size, 0, 0, &result);
    /* Possible weaknesses found:
     *  Condition 'ret==0' is always false
     *  Condition 'ret==0' is always false [knownConditionTrueFalse]
     */
    if (ret == 0) {
        printf("%d\n", result);
    } else {
        printf("Error: %d\n", ret);
    }

    ret = extract_nth_element(list, list_size, 3, 0, &result);
    if (ret == 0) {
        printf("%d\n", result);
    } else {
        printf("Error: %d\n", ret);
    }

    ret = extract_nth_element(list, list_size, 0, 5, &result);
    if (ret == 0) {
        printf("%d\n", result);
    } else {
        printf("Error: %d\n", ret);
    }

    return 0;
}