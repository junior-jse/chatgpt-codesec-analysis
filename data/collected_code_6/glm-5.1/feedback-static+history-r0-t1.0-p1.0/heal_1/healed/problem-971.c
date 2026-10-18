#include <stdio.h>
#include <stdlib.h>

int find_maximum_segments(int n, int a, int b, int c) {
    if (n < 0) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }

    int res_a = find_maximum_segments(n - a, a, b, c);
    int res_b = find_maximum_segments(n - b, a, b, c);
    int res_c = find_maximum_segments(n - c, a, b, c);

    int max_res = res_a;
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

int main(int argc, char * const argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <n> <a> <b> <c>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    long val = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || val < 0) {
        fprintf(stderr, "Invalid input for n\n");
        return EXIT_FAILURE;
    }
    int n = (int)val;

    val = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || val <= 0) {
        fprintf(stderr, "Invalid input for a\n");
        return EXIT_FAILURE;
    }
    int a = (int)val;

    val = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0' || val <= 0) {
        fprintf(stderr, "Invalid input for b\n");
        return EXIT_FAILURE;
    }
    int b = (int)val;

    val = strtol(argv[4], &endptr, 10);
    if (*endptr != '\0' || val <= 0) {
        fprintf(stderr, "Invalid input for c\n");
        return EXIT_FAILURE;
    }
    int c = (int)val;

    int result = find_maximum_segments(n, a, b, c);
    printf("%d\n", result);

    return EXIT_SUCCESS;
}