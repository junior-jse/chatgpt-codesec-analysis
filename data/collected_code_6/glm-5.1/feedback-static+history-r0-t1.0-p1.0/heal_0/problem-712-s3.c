#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int len;
} List;

typedef struct {
    List *lists;
    int count;
} ListOfLists;

int lists_equal(List a, List b) {
    if (a.len != b.len) return 0;
    for (int i = 0; i < a.len; i++) {
        if (a.data[i] != b.data[i]) return 0;
    }
    return 1;
}

ListOfLists remove_duplicates(ListOfLists input) {
    ListOfLists result;
    result.count = 0;
    result.lists = (List *)malloc(input.count * sizeof(List));
    if (!result.lists) {
        return result;
    }

    for (int i = 0; i < input.count; i++) {
        int is_dup = 0;
        for (int j = 0; j < result.count; j++) {
            if (lists_equal(input.lists[i], result.lists[j])) {
                is_dup = 1;
                break;
            }
        }

        if (!is_dup) {
            List new_list;
            new_list.len = input.lists[i].len;
            new_list.data = (int *)malloc(new_list.len * sizeof(int));
            if (!new_list.data) {
                for (int k = 0; k < result.count; k++) {
                    free(result.lists[k].data);
                }
                free(result.lists);
                result.lists = NULL;
                result.count = 0;
                return result;
            }
            for (int k = 0; k < new_list.len; k++) {
                new_list.data[k] = input.lists[i].data[k];
            }
            result.lists[result.count++] = new_list;
        }
    }

    return result;
}

void free_list_of_lists(ListOfLists lol) {
    if (lol.lists) {
        for (int i = 0; i < lol.count; i++) {
            free(lol.lists[i].data);
        }
        free(lol.lists);
    }
}

int main() {
    List lists[5];
    int data0[] = {1, 2, 3};
    int data1[] = {4, 5};
    int data2[] = {1, 2, 3};
    int data3[] = {6};
    int data4[] = {4, 5};

    lists[0].data = data0; lists[0].len = 3;
    lists[1].data = data1; lists[1].len = 2;
    lists[2].data = data2; lists[2].len = 3;
    lists[3].data = data3; lists[3].len = 1;
    lists[4].data = data4; lists[4].len = 2;

    ListOfLists input;
    input.lists = lists;
    input.count = 5;

    ListOfLists output = remove_duplicates(input);

    for (int i = 0; i < output.count; i++) {
        for (int j = 0; j < output.lists[i].len; j++) {
            printf("%d ", output.lists[i].data[j]);
        }
        printf("\n");
    }

    free_list_of_lists(output);

    return 0;
}