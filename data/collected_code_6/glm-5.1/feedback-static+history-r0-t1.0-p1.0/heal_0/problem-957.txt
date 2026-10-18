#include <stdio.h>
#include <stdlib.h>

int get_rightmost_set_bit_pos(int n) {
    if (n == 0) {
        return -1;
    }
    unsigned int mask = (unsigned int)n;
    mask = mask & (-mask);
    int pos = 0;
    while (mask > 1) {
        mask >>= 1;
        pos++;
    }
    return pos;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    long val = strtol(argv[1], NULL, 10);
    if (val < INT_MIN || val > INT_MAX) {
        return EXIT_FAILURE;
    }
    int n = (int)val;
    int pos = get_rightmost_set_bit_pos(n);
    printf("%d\n", pos);
    return EXIT_SUCCESS;
}