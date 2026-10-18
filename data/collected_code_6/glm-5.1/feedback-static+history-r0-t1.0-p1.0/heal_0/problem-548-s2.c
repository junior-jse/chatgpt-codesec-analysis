#include <stdio.h>
#include <stdlib.h>

int longest_increasing_subsequence(int *sequence, int length) {
    if (sequence == NULL || length <= 0) {
        return 0;
    }

    int *tails = (int *)malloc(length * sizeof(int));
    if (tails == NULL) {
        return 0;
    }

    int size = 0;

    for (int i = 0; i < length; i++) {
        int low = 0;
        int high = size;

        while (low < high) {
            int mid = low + (high - low) / 2;
            if (tails[mid] < sequence[i]) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }

        tails[low] = sequence[i];

        if (low == size) {
            size++;
        }
    }

    free(tails);
    return size;
}

int main() {
    int sequence[] = {10, 9, 2, 5, 3, 7, 101, 18};
    int length = sizeof(sequence) / sizeof(sequence[0]);

    int result = longest_increasing_subsequence(sequence, length);

    printf("%d\n", result);

    return 0;
}