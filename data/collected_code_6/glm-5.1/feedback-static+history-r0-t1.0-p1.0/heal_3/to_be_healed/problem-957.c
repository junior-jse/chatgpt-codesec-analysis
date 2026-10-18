#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
 */
int main(int argc, char * const argv[const]) {
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