#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void findClosest(int *a, int n1, int *b, int n2, int *c, int n3) {
    int i = 0, j = 0, k = 0;
    int res1, res2, res3;
    int min_diff = INT_MAX;

    while (i < n1 && j < n2 && k < n3) {
        int mn = a[i];
        if (b[j] < mn) mn = b[j];
        if (c[k] < mn) mn = c[k];

        int mx = a[i];
        if (b[j] > mx) mx = b[j];
        if (c[k] > mx) mx = c[k];

        int curr_diff = mx - mn;

        if (curr_diff < min_diff) {
            min_diff = curr_diff;
            res1 = a[i];
            res2 = b[j];
            res3 = c[k];
        }

        if (curr_diff == 0) break;

        if (a[i] == mn) i++;
        else if (b[j] == mn) j++;
        else k++;
    }

    printf("%d %d %d\n", res1, res2, res3);
}

int main() {
    int a[] = {1, 4, 10};
    int b[] = {2, 15, 20};
    int c[] = {10, 12};

    findClosest(a, 3, b, 3, c, 2);

    return 0;
}