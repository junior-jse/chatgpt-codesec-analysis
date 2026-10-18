#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int rotate_left(int *arr, size_t n, size_t k) {
    if (arr == NULL || n == 0) {
        return -1;
    }

    k = k % n;
    if (k == 0) {
        return 0;
    }

    if (n > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    size_t total_bytes = n * sizeof(int);
    size_t k_bytes = k * sizeof(int);
    size_t remaining_bytes = (n - k) * sizeof(int);

    /* Possible weaknesses found:
     *  Assuming that condition 'remaining_bytes>total_bytes' is not redundant
     *  Assuming that condition 'k_bytes>total_bytes' is not redundant
     */
    if (k_bytes > total_bytes || remaining_bytes > total_bytes) {
        return -1;
    }

    int *temp = malloc(k_bytes);
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (temp == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     *  Condition 'k_bytes<=total_bytes' is always true [knownConditionTrueFalse]
     *  Condition 'k_bytes<=total_bytes' is always true
     */
    if (k_bytes <= total_bytes) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(temp, arr, k_bytes);
    } else {
        free(temp);
        return -1;
    }

    /* Possible weaknesses found:
     *  Condition 'remaining_bytes<=total_bytes' is always true [knownConditionTrueFalse]
     *  Condition 'remaining_bytes<=total_bytes' is always true
     */
    if (remaining_bytes <= total_bytes) {
        memmove(arr, arr + k, remaining_bytes);
    } else {
        free(temp);
        return -1;
    }

    if (k_bytes <= total_bytes - remaining_bytes) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(arr + n - k, temp, k_bytes);
    } else {
        free(temp);
        return -1;
    }

    free(temp);
    return 0;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    size_t k = 3;

    if (rotate_left(arr, n, k) != 0) {
        fprintf(stderr, "Rotation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}