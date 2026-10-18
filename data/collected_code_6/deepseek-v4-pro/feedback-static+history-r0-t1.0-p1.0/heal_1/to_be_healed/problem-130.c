#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int count;
} Frequency;

/* Possible weaknesses found:
 *  Parameter 'list' can be declared as pointer to const [constParameterPointer]
 */
int find_max_frequency_item(int *list, int size) {
    if (list == NULL || size <= 0) {
        return -1;
    }

    Frequency *freq = (Frequency *)malloc(size * sizeof(Frequency));
    if (freq == NULL) {
        return -1;
    }

    int unique_count = 0;

    for (int i = 0; i < size; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (freq[j].value == list[i]) {
                freq[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            freq[unique_count].value = list[i];
            freq[unique_count].count = 1;
            unique_count++;
        }
    }

    int max_freq = 0;
    int max_item = freq[0].value;

    for (int i = 0; i < unique_count; i++) {
        if (freq[i].count > max_freq) {
            max_freq = freq[i].count;
            max_item = freq[i].value;
        }
    }

    free(freq);
    return max_item;
}

int main(void) {
    int list[] = {1, 3, 2, 1, 4, 1, 3, 2, 1};
    int size = sizeof(list) / sizeof(list[0]);
    int result = find_max_frequency_item(list, size);

    if (result != -1) {
        printf("Item with maximum frequency: %d\n", result);
    } else {
        printf("Error: Invalid input or allocation failure\n");
    }

    return 0;
}