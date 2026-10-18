#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_max_frequency_item(const int *list, size_t size) {
    if (list == NULL || size == 0) {
        return INT_MIN;
    }

    int *items = NULL;
    size_t *freqs = NULL;
    size_t unique_count = 0;
    int max_item = list[0];
    size_t max_freq = 0;

    items = (int *)malloc(size * sizeof(int));
    if (items == NULL) {
        return INT_MIN;
    }

    freqs = (size_t *)malloc(size * sizeof(size_t));
    if (freqs == NULL) {
        free(items);
        return INT_MIN;
    }

    for (size_t i = 0; i < size; ++i) {
        int found = 0;
        for (size_t j = 0; j < unique_count; ++j) {
            if (items[j] == list[i]) {
                freqs[j]++;
                found = 1;
                break;
            }
        }

        if (!found) {
            items[unique_count] = list[i];
            freqs[unique_count] = 1;
            unique_count++;
        }
    }

    for (size_t i = 0; i < unique_count; ++i) {
        if (freqs[i] > max_freq) {
            max_freq = freqs[i];
            max_item = items[i];
        }
    }

    free(items);
    free(freqs);

    return max_item;
}

int main(void) {
    int list[] = {1, 3, 2, 1, 4, 1, 3, 2, 1};
    size_t size = sizeof(list) / sizeof(list[0]);

    int result = find_max_frequency_item(list, size);

    printf("%d\n", result);

    return 0;
}