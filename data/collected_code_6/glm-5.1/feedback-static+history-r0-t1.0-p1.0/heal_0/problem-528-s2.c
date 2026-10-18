#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t len;
} List;

typedef struct {
    List *lists;
    size_t count;
} ListOfLists;

ListOfLists find_min_length_lists(ListOfLists input) {
    ListOfLists result;
    result.lists = NULL;
    result.count = 0;

    if (input.lists == NULL || input.count == 0) {
        return result;
    }

    size_t min_len = input.lists[0].len;
    for (size_t i = 1; i < input.count; i++) {
        if (input.lists[i].len < min_len) {
            min_len = input.lists[i].len;
        }
    }

    size_t capacity = 10;
    result.lists = (List *)malloc(capacity * sizeof(List));
    if (result.lists == NULL) {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < input.count; i++) {
        if (input.lists[i].len == min_len) {
            if (result.count >= capacity) {
                capacity *= 2;
                List *temp = (List *)realloc(result.lists, capacity * sizeof(List));
                if (temp == NULL) {
                    free(result.lists);
                    exit(EXIT_FAILURE);
                }
                result.lists = temp;
            }
            result.lists[result.count] = input.lists[i];
            result.count++;
        }
    }

    if (result.count < capacity) {
        List *temp = (List *)realloc(result.lists, result.count * sizeof(List));
        if (temp != NULL || result.count == 0) {
            result.lists = temp;
        }
    }

    return result;
}

int main(void) {
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5};
    int a3[] = {6};
    int a4[] = {7, 8};

    List l1 = {a1, 3};
    List l2 = {a2, 2};
    List l3 = {a3, 1};
    List l4 = {a4, 2};

    List input_lists[] = {l1, l2, l3, l4};
    ListOfLists in = {input_lists, 4};

    ListOfLists out = find_min_length_lists(in);

    for (size_t i = 0; i < out.count; i++) {
        for (size_t j = 0; j < out.lists[i].len; j++) {
            printf("%d ", out.lists[i].data[j]);
        }
        printf("\n");
    }

    free(out.lists);

    return 0;
}