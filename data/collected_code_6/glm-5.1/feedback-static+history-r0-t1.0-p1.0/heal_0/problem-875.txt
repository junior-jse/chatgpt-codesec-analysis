#include <stdio.h>
#include <stdlib.h>

int min_diff_tuples(int *t1, int *t2, int n) {
    if (n <= 0) return -1;
    int min_val = abs(t1[0] - t2[0]);
    for (int i = 1; i < n; i++) {
        int diff = abs(t1[i] - t2[i]);
        if (diff < min_val) min_val = diff;
    }
    return min_val;
}

int main() {
    int t1[] = {10, 20, 30, 40};
    int t2[] = {12, 25, 31, 50};
    int n = sizeof(t1) / sizeof(t1[0]);
    int result = min_diff_tuples(t1, t2, n);
    printf("%d\n", result);
    return 0;
}