#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int max_bitonic_subseq_sum(const int arr[], int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return arr[0];
    }

    int *msi = (int *)malloc(n * sizeof(int));
    int *msd = (int *)malloc(n * sizeof(int));

    if (msi == NULL || msd == NULL) {
        free(msi);
        free(msd);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        msi[i] = arr[i];
        msd[i] = arr[i];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i] && msi[i] < msi[j] + arr[i]) {
                msi[i] = msi[j] + arr[i];
            }
        }
    }

    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (arr[j] < arr[i] && msd[i] < msd[j] + arr[i]) {
                msd[i] = msd[j] + arr[i];
            }
        }
    }

    int max_sum = msi[0] + msd[0] - arr[0];
    for (int i = 1; i < n; i++) {
        if (msi[i] + msd[i] - arr[i] > max_sum) {
            max_sum = msi[i] + msd[i] - arr[i];
        }
    }

    free(msi);
    free(msd);

    return max_sum;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc < 2) {
        return 1;
    }

    int n = argc - 1;
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        char *endptr;
        errno = 0;
        long val = strtol(argv[i + 1], &endptr, 10);
        if (errno != 0 || endptr == argv[i + 1] || *endptr != '\0' || val > INT_MAX || val < INT_MIN) {
            free(arr);
            return 1;
        }
        arr[i] = (int)val;
    }

    int result = max_bitonic_subseq_sum(arr, n);
    if (result != -1) {
        printf("%d\n", result);
    }

    free(arr);

    return 0;
}