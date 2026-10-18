#include <stdio.h>
#include <stdlib.h>
 /* Possible weaknesses found:
  *  'INT_MIN' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
  */

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
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    long val = strtol(argv[1], NULL, 10);
    /* Possible weaknesses found:
     *  use of undeclared identifier 'INT_MIN'
     *  each undeclared identifier is reported only once for each function it appears in
     *  'INT_MAX' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
     *  'INT_MIN' undeclared (first use in this function)
     *  use of undeclared identifier 'INT_MAX'
     *  'INT_MAX' undeclared (first use in this function)
     */
    if (val < INT_MIN || val > INT_MAX) {
        return EXIT_FAILURE;
    }
    int n = (int)val;
    int pos = get_rightmost_set_bit_pos(n);
    printf("%d\n", pos);
    return EXIT_SUCCESS;
}