#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_max_frequency_item(int *list, size_t size) {
    if (list == NULL || size == 0) {
        return INT_MIN;
    }

    int max_item = list[0];
    int max_count = 1;

    for (size_t i = 0; i < size; i++) {
        if (list[i] == INT_MIN) {
            continue;
        }

        int current_count = 1;
        for (size_t j = i + 1; j < size; j++) {
            if (list[i] == list[j]) {
                current_count++;
                list[j] = INT_MIN;
            }
        }

        if (current_count > max_count) {
            max_count = current_count;
            max_item = list[i];
        }
    }

    return max_item;
}

int main(int argc, char * const argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    size_t size = argc - 1;
    int *list = malloc(size * sizeof(int));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < size; i++) {
        long val = strtol(argv[i + 1], NULL, 10);
        if (val > INT_MAX || val < INT_MIN) {
            free(list);
            return EXIT_FAILURE;
        }
        list[i] = (int)val;
    }

    int result = find_max_frequency_item(list, size);

    printf("%d\n", result);

    free(list);

    return EXIT_SUCCESS;
}