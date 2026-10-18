#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void find_three_closest(const int *a, int n1, const int *b, int n2, const int *c, int n3, int *res) {
    int i = 0, j = 0, k = 0;
    int min_diff = INT_MAX;

    if (a == NULL || b == NULL || c == NULL || n1 <= 0 || n2 <= 0 || n3 <= 0) {
        res[0] = -1;
        res[1] = -1;
        res[2] = -1;
        return;
    }

    while (i < n1 && j < n2 && k < n3) {
        int current_max = a[i];
        if (b[j] > current_max) current_max = b[j];
        if (c[k] > current_max) current_max = c[k];

        int current_min = a[i];
        if (b[j] < current_min) current_min = b[j];
        if (c[k] < current_min) current_min = c[k];

        int diff = current_max - current_min;

        if (diff < min_diff) {
            min_diff = diff;
            res[0] = a[i];
            res[1] = b[j];
            res[2] = c[k];
        }

        if (min_diff == 0) {
            break;
        }

        if (current_min == a[i]) {
            i++;
        } else if (current_min == b[j]) {
            j++;
        } else {
            k++;
        }
    }
}

int main() {
    int a[] = {1, 4, 10};
    int b[] = {2, 15, 20};
    int c[] = {10, 12};
    int n1 = sizeof(a) / sizeof(a[0]);
    int n2 = sizeof(b) / sizeof(b[0]);
    int n3 = sizeof(c) / sizeof(c[0]);
    int res[3];

    find_three_closest(a, n1, b, n2, c, n3, res);

    printf("%d %d %d\n", res[0], res[1], res[2]);

    return 0;
}