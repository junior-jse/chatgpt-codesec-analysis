#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int frequency;
} FrequencyItem;

int find_max_frequency_item(const int *list, int length, int *result) {
    if (list == NULL || length <= 0 || result == NULL) {
        return 0;
    }

    FrequencyItem *items = (FrequencyItem *)malloc(length * sizeof(FrequencyItem));
    if (items == NULL) {
        return 0;
    }

    int unique_count = 0;
    int i, j;

    for (i = 0; i < length; i++) {
        int found = 0;
        for (j = 0; j < unique_count; j++) {
            if (items[j].value == list[i]) {
                items[j].frequency++;
                found = 1;
                break;
            }
        }
        if (!found) {
            items[unique_count].value = list[i];
            items[unique_count].frequency = 1;
            unique_count++;
        }
    }

    int max_freq = 0;
    int max_index = 0;
    for (i = 0; i < unique_count; i++) {
        if (items[i].frequency > max_freq) {
            max_freq = items[i].frequency;
            max_index = i;
        }
    }

    *result = items[max_index].value;
    free(items);
    return 1;
}

int main(void) {
    int list[] = {1, 3, 5, 3, 7, 9, 3, 5, 5, 5, 11, 13, 5};
    int length = sizeof(list) / sizeof(list[0]);
    int result;

    if (find_max_frequency_item(list, length, &result)) {
        printf("Item with maximum frequency: %d\n", result);
    } else {
        printf("Error: invalid input or memory allocation failure\n");
    }

    const int list2[] = {42};
    if (find_max_frequency_item(list2, 1, &result)) {
        printf("Item with maximum frequency: %d\n", result);
    }

    const int list3[] = {7, 7, 8, 8, 8, 7, 7, 9};
    if (find_max_frequency_item(list3, 8, &result)) {
        printf("Item with maximum frequency: %d\n", result);
    }

    return 0;
}