#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void find_three_closest(const int arr1[], int n1, const int arr2[], int n2, const int arr3[], int n3, int result[]) {
    int i = 0, j = 0, k = 0;
    int min_diff = INT_MAX;

    while (i < n1 && j < n2 && k < n3) {
        int a = arr1[i];
        int b = arr2[j];
        int c = arr3[k];

        int max_val = a > b ? (a > c ? a : c) : (b > c ? b : c);
        int min_val = a < b ? (a < c ? a : c) : (b < c ? b : c);
        int current_diff = max_val - min_val;

        if (current_diff < min_diff) {
            min_diff = current_diff;
            result[0] = a;
            result[1] = b;
            result[2] = c;
        }

        if (min_diff == 0) {
            break;
        }

        if (a <= b && a <= c) {
            i++;
        } else if (b <= a && b <= c) {
            j++;
        } else {
            k++;
        }
    }
}

int main() {
    int arr1[] = {1, 4, 10};
    int arr2[] = {2, 15, 20};
    int arr3[] = {10, 12};

    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int n3 = sizeof(arr3) / sizeof(arr3[0]);

    int result[3] = {0, 0, 0};

    find_three_closest(arr1, n1, arr2, n2, arr3, n3, result);

    printf("%d %d %d\n", result[0], result[1], result[2]);

    return 0;
}