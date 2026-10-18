#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int frequency;
} FrequencyItem;

int findMaxFrequencyItem(const int *list, int size, int *result) {
    if (list == NULL || size <= 0) {
        return 0;
    }

    FrequencyItem *items = (FrequencyItem *)malloc(size * sizeof(FrequencyItem));
    if (items == NULL) {
        return 0;
    }

    int itemCount = 0;
    int i, j, found;

    for (i = 0; i < size; i++) {
        found = 0;
        for (j = 0; j < itemCount; j++) {
            if (items[j].value == list[i]) {
                items[j].frequency++;
                found = 1;
                break;
            }
        }
        if (!found) {
            items[itemCount].value = list[i];
            items[itemCount].frequency = 1;
            itemCount++;
        }
    }

    int maxFreq = 0;
    int maxIndex = 0;
    for (i = 0; i < itemCount; i++) {
        if (items[i].frequency > maxFreq) {
            maxFreq = items[i].frequency;
            maxIndex = i;
        }
    }

    *result = items[maxIndex].value;
    int maxFrequency = items[maxIndex].frequency;

    free(items);
    return maxFrequency;
}

int main(void) {
    int list1[] = {1, 3, 2, 1, 4, 1, 3, 1, 5, 1};
    int size1 = sizeof(list1) / sizeof(list1[0]);
    int result1;
    int freq1 = findMaxFrequencyItem(list1, size1, &result1);
    printf("List 1 - Max frequency item: %d, Frequency: %d\n", result1, freq1);

    int list2[] = {5, 5, 5, 2, 2, 2, 2, 8, 8};
    int size2 = sizeof(list2) / sizeof(list2[0]);
    int result2;
    int freq2 = findMaxFrequencyItem(list2, size2, &result2);
    printf("List 2 - Max frequency item: %d, Frequency: %d\n", result2, freq2);

    int list3[] = {7};
    int size3 = sizeof(list3) / sizeof(list3[0]);
    int result3;
    int freq3 = findMaxFrequencyItem(list3, size3, &result3);
    printf("List 3 - Max frequency item: %d, Frequency: %d\n", result3, freq3);

    int result4;
    int freq4 = findMaxFrequencyItem(NULL, 0, &result4);
    printf("List 4 - Result: %d, Frequency: %d\n", result4, freq4);

    int list5[] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 1, 2, 3, 1, 2, 1};
    int size5 = sizeof(list5) / sizeof(list5[0]);
    int result5;
    int freq5 = findMaxFrequencyItem(list5, size5, &result5);
    printf("List 5 - Max frequency item: %d, Frequency: %d\n", result5, freq5);

    return 0;
}