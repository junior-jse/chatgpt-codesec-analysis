#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_reverse(const char *str1, const char *str2) {
    if (strlen(str1) != strlen(str2)) {
        return 0;
    }
    int len = strlen(str1);
    for (int i = 0; i < len; i++) {
        if (str1[i] != str2[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

int count_reverse_pairs(char **str_list, int list_size) {
    if (str_list == NULL || list_size <= 1) {
        return 0;
    }

    int *used = (int *)calloc(list_size, sizeof(int));
    if (used == NULL) {
        return -1;
    }

    int count = 0;
    for (int i = 0; i < list_size; i++) {
        if (used[i]) {
            continue;
        }
        for (int j = i + 1; j < list_size; j++) {
            if (used[j]) {
                continue;
            }
            if (is_reverse(str_list[i], str_list[j])) {
                count++;
                used[i] = 1;
                used[j] = 1;
                break;
            }
        }
    }

    free(used);
    return count;
}

int main(void) {
    char *test_list[] = {
        "abc", "cba", "hello", "olleh", "world", "dlrow", "test", "xyz"
    };
    int size = sizeof(test_list) / sizeof(test_list[0]);

    int result = count_reverse_pairs(test_list, size);
    if (result >= 0) {
        printf("Number of reverse pairs: %d\n", result);
    } else {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    char *empty_list[] = {};
    int empty_size = 0;
    int empty_result = count_reverse_pairs(empty_list, empty_size);
    printf("Empty list result: %d\n", empty_result);

    char *single_list[] = {"abc"};
    int single_size = 1;
    int single_result = count_reverse_pairs(single_list, single_size);
    printf("Single element list result: %d\n", single_result);

    char *null_list[] = {NULL, "abc", NULL, "cba"};
    int null_size = 4;
    int null_result = count_reverse_pairs(null_list, null_size);
    if (null_result >= 0) {
        printf("List with NULL entries result: %d\n", null_result);
    } else {
        printf("List with NULL entries result: error handled\n");
    }

    return EXIT_SUCCESS;
}