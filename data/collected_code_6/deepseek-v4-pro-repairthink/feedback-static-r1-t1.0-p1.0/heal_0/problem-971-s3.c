#include <stdio.h>
#include <limits.h>

int maxSegments(int n, int a, int b, int c) {
    if (n == 0) return 0;
    if (n < 0) return -1;

    int res_a = maxSegments(n - a, a, b, c);
    int res_b = maxSegments(n - b, a, b, c);
    int res_c = maxSegments(n - c, a, b, c);

    int max = res_a;
    if (res_b > max) max = res_b;
    if (res_c > max) max = res_c;

    if (max == -1) return -1;
    return max + 1;
}

int main(void) {
    int n, a, b, c;

    printf("Enter n: ");
    if (scanf("%d", &n) != 1) return 1;

    printf("Enter a, b, c: ");
    if (scanf("%d %d %d", &a, &b, &c) != 3) return 1;

    if (a <= 0 || b <= 0 || c <= 0 || n < 0) {
        printf("Invalid input\n");
        return 1;
    }

    int result = maxSegments(n, a, b, c);
    if (result == -1) {
        printf("Cannot form segments\n");
    } else {
        printf("Maximum segments: %d\n", result);
    }

    return 0;
}