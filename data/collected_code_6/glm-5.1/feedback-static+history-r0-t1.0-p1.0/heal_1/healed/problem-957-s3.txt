#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned int rightmost_set_bit_position(int num) {
    if (num == 0) {
        return 0;
    }
    unsigned int n = (unsigned int)num;
    unsigned int isolated = n & (~n + 1);
    unsigned int pos = 0;
    while (isolated >>= 1) {
        pos++;
    }
    return pos + 1;
}

int main(int argc, char * const argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    char *endptr;
    long val = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || val < INT_MIN || val > INT_MAX) {
        return EXIT_FAILURE;
    }
    int num = (int)val;
    unsigned int res = rightmost_set_bit_position(num);
    printf("%u\n", res);
    return EXIT_SUCCESS;
}