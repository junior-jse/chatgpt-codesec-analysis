#include <stdio.h>
#include <stdlib.h>

int find_max_frequency_item(const int *list, size_t size) {
    if (list == NULL || size == 0) {
        return 0;
    }

    int min_val = list[0];
    int max_val = list[0];
    size_t i;

    for (i = 1; i < size; i++) {
        if (list[i] < min_val) {
            min_val = list[i];
        }
        if (list[i] > max_val) {
            max_val = list[i];
        }
    }

    size_t range = (size_t)(max_val - min_val + 1);

    size_t *counts = (size_t *)calloc(range, sizeof(size_t));
    if (counts == NULL) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        counts[list[i] - min_val]++;
    }

    size_t max_count = 0;
    int max_freq_item = list[0];

    for (i = 0; i < range; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_freq_item = (int)i + min_val;
        }
    }

    free(counts);

    return max_freq_item;
}

int main(void) {
    int list1[] = {1, 3, 2, 1, 4, 1, 3, 3, 3};
    size_t size1 = sizeof(list1) / sizeof(list1[0]);
    
    int result1 = find_max_frequency_item(list1, size1);
    printf("%d\n", result1);

    int list2[] = {-5, -5, -2, -2, -2, -5};
    size_t size2 = sizeof(list2) / sizeof(list2[0]);
    
    int result2 = find_max_frequency_item(list2, size2);
    printf("%d\n", result2);

    /* Possible weaknesses found:
     *  Variable 'list3' can be declared as pointer to const [constVariablePointer]
     */
    int *list3 = NULL;
    size_t size3 = 0;
    
    int result3 = find_max_frequency_item(list3, size3);
    printf("%d\n", result3);

    return 0;
}