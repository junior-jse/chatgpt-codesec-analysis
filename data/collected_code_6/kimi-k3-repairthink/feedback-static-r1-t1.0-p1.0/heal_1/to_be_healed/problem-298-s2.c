#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'list' can be declared as pointer to const [constParameterPointer]
 */
int is_present(int *list, int list_size, int value) {
    int i;
    if (list == NULL || list_size <= 0) {
        return 0;
    }
    for (i = 0; i < list_size; i++) {
        if (list[i] == value) {
            return 1;
        }
    }
    return 0;
}

/* Possible weaknesses found:
 *  Parameter 'sub_sizes' can be declared as pointer to const [constParameterPointer]
 */
int *find_nested_elements(int **nested_list, int *sub_sizes, int nested_count, int *other_list, int other_size, int *result_count) {
    int i, j;
    int capacity = 16;
    int count = 0;
    int *result = NULL;
    int *temp = NULL;

    if (nested_list == NULL || sub_sizes == NULL || other_list == NULL || result_count == NULL || nested_count < 0 || other_size < 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    result = (int *)malloc((size_t)capacity * sizeof(int));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    for (i = 0; i < nested_count; i++) {
        if (nested_list[i] == NULL || sub_sizes[i] < 0) {
            continue;
        }
        for (j = 0; j < sub_sizes[i]; j++) {
            if (is_present(other_list, other_size, nested_list[i][j])) {
                if (count == capacity) {
                    capacity *= 2;
                    temp = (int *)realloc(result, (size_t)capacity * sizeof(int));
                    if (temp == NULL) {
                        free(result);
                        *result_count = 0;
                        return NULL;
                    }
                    result = temp;
                }
                result[count] = nested_list[i][j];
                count++;
            }
        }
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    temp = (int *)realloc(result, (size_t)count * sizeof(int));
    if (temp != NULL) {
        result = temp;
    }

    *result_count = count;
    return result;
}

int main(void) {
    int sub1[] = {1, 2, 3};
    int sub2[] = {4, 5, 6};
    int sub3[] = {7, 8, 9};
    int *nested_list[] = {sub1, sub2, sub3};
    int sub_sizes[] = {3, 3, 3};
    int other_list[] = {2, 4, 6, 8, 10};
    int other_size = 5;
    int result_count = 0;
    int *result = NULL;
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    int i;

    result = find_nested_elements(nested_list, sub_sizes, 3, other_list, other_size, &result_count);

    if (result != NULL) {
        for (i = 0; i < result_count; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No common elements found or error occurred.\n");
    }

    return 0;
}