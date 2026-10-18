#include <stdio.h>
#include <stdlib.h>

int longest_increasing_subsequence(const int *sequence, size_t size) {
    if (sequence == NULL || size == 0) {
        return 0;
    }

    int *tails = (int *)malloc(size * sizeof(int));
    if (tails == NULL) {
        return -1;
    }

    size_t length = 0;

    for (size_t i = 0; i < size; i++) {
        size_t left = 0;
        size_t right = length;

        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (tails[mid] < sequence[i]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        tails[left] = sequence[i];
        if (left == length) {
            length++;
        }
    }

    free(tails);
    return (int)length;
}

int main(void) {
    int sequence[] = {10, 9, 2, 5, 3, 7, 101, 18};
    size_t size = sizeof(sequence) / sizeof(sequence[0]);

    int result = longest_increasing_subsequence(sequence, size);
    if (result >= 0) {
        printf("Length of longest increasing subsequence: %d\n", result);
    } else {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}