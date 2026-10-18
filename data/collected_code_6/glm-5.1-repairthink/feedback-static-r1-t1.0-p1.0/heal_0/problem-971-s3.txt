#include <stdio.h>
#include <stdlib.h>

int find_max_segments(int n, int a, int b, int c) {
    if (n < 0) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }

    int res_a = find_max_segments(n - a, a, b, c);
    int res_b = find_max_segments(n - b, a, b, c);
    int res_c = find_max_segments(n - c, a, b, c);

    int max_res = -1;
    if (res_a > max_res) {
        max_res = res_a;
    }
    if (res_b > max_res) {
        max_res = res_b;
    }
    if (res_c > max_res) {
        max_res = res_c;
    }

    if (max_res == -1) {
        return -1;
    }

    return max_res + 1;
}

int find_max_segments_wrapper(int n, int a, int b, int c) {
    if (n <= 0 || a <= 0 || b <= 0 || c <= 0) {
        return -1;
    }
    return find_max_segments(n, a, b, c);
}

int main(void) {
    int n = 7;
    int a = 5;
    int b = 2;
    int c = 5;

    int result = find_max_segments_wrapper(n, a, b, c);

    printf("%d\n", result);

    return 0;
}