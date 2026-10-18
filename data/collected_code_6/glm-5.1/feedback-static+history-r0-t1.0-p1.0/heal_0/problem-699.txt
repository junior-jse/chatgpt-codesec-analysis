#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min_swaps_binary_strings(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) {
        return -1;
    }

    size_t len = len1;
    size_t mismatch_1_0 = 0;
    size_t mismatch_0_1 = 0;

    for (size_t i = 0; i < len; i++) {
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <string1> <string2>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *s1 = argv[1];
    const char *s2 = argv[2];

    for (size_t i = 0; i < strlen(s1); i++) {
        if (s1[i] != '0' && s1[i] != '1') {
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < strlen(s2); i++) {
        if (s2[i] != '0' && s2[i] != '1') {
            return EXIT_FAILURE;
        }
    }

    int result = min_swaps_binary_strings(s1, s2);

    if (result == -1) {
        printf("Impossible\n");
    } else {
        printf("%d\n", result);
    }

    return EXIT_SUCCESS;
}