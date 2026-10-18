#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Possible weaknesses found:
 *  expected 'size_t' {aka 'long unsigned int'} but argument is of type 'const char *'
 *  declared here
 *  'min_swaps_binary_strings' declared here
 */
int min_swaps_binary_strings(const char *s1, size_t len) {
    size_t mismatch_1_0 = 0;
    size_t mismatch_0_1 = 0;

    for (size_t i = 0; i < len; i++) {
        /* Possible weaknesses found:
         *  use of undeclared identifier 's2'
         *  each undeclared identifier is reported only once for each function it appears in
         *  's2' undeclared (first use in this function); did you mean 's1'?
         */
        if (s1[i] != s2[i]) {
            if (s1[i] == '1') {
                mismatch_1_0++;
            } else {
                mismatch_0_1++;
            }
        }
    }

    if (mismatch_1_0 != mismatch_0_1) {
        return -1;
    }

    if (mismatch_1_0 == 0) {
        return 0;
    }

    return (int)mismatch_1_0;
}

int main(int argc, char * const argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <string1> <string2>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *s1 = argv[1];
    const char *s2 = argv[2];

    size_t len1 = strnlen(s1, SIZE_MAX);
    size_t len2 = strnlen(s2, SIZE_MAX);

    if (len1 == 0 || len2 == 0 || len1 != len2) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < len1; i++) {
        if (s1[i] != '0' && s1[i] != '1') {
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < len2; i++) {
        if (s2[i] != '0' && s2[i] != '1') {
            return EXIT_FAILURE;
        }
    }

    /* Possible weaknesses found:
     *  too many arguments to function call, expected 2, have 3
     *  passing argument 2 of 'min_swaps_binary_strings' makes integer from pointer without a cast [-Wint-conversion]
     *  too many arguments to function 'min_swaps_binary_strings'; expected 2, have 3
     */
    int result = min_swaps_binary_strings(s1, s2, len1);

    if (result == -1) {
        printf("Impossible\n");
    } else {
        printf("%d\n", result);
    }

    return EXIT_SUCCESS;
}