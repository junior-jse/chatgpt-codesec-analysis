#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int findClosestTriplet(const int *arr1, int n1, const int *arr2, int n2, const int *arr3, int n3, int *res1, int *res2, int *res3) {
    if (arr1 == NULL || arr2 == NULL || arr3 == NULL || res1 == NULL || res2 == NULL || res3 == NULL) {
        return 0;
    }
    if (n1 <= 0 || n2 <= 0 || n3 <= 0) {
        return 0;
    }

    int i = 0, j = 0, k = 0;
    int min_diff = INT_MAX;
    int found = 0;

    while (i < n1 && j < n2 && k < n3) {
        int minimum = arr1[i];
        if (arr2[j] < minimum) minimum = arr2[j];
        if (arr3[k] < minimum) minimum = arr3[k];

        int maximum = arr1[i];
        if (arr2[j] > maximum) maximum = arr2[j];
        if (arr3[k] > maximum) maximum = arr3[k];

        int diff = maximum - minimum;
        if (diff < 0) diff = -diff;

        if (diff < min_diff) {
            min_diff = diff;
            *res1 = arr1[i];
            *res2 = arr2[j];
            *res3 = arr3[k];
            found = 1;
        }

        if (min_diff == 0) {
            break;
        }

        if (arr1[i] <= arr2[j] && arr1[i] <= arr3[k]) {
            i++;
        } else if (arr2[j] <= arr1[i] && arr2[j] <= arr3[k]) {
            j++;
        } else {
            k++;
        }
    }

    return found;
}

int main(void) {
    int arr1[] = {1, 4, 10};
    int arr2[] = {2, 15, 20};
    int arr3[] = {10, 12};

    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int n3 = sizeof(arr3) / sizeof(arr3[0]);

    int res1, res2, res3;

    if (findClosestTriplet(arr1, n1, arr2, n2, arr3, n3, &res1, &res2, &res3)) {
        printf("%d %d %d\n", res1, res2, res3);
    } else {
        printf("No valid triplet found\n");
    }

    return 0;
}