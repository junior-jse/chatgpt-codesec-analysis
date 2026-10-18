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

    if (element_index >= sizeof(Tuple) / sizeof(int)) {
        return -1;
    }

    const int *tuple_array = (const int *)(&list->data[n]);
    *out_value = tuple_array[element_index];

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

    ret = extract_nth_element(NULL, 0, 0, &value);
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