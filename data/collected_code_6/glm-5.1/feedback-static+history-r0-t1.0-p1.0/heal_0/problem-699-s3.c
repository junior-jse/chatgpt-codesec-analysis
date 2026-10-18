#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min_swaps_binary(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) {
        return -1;
    }

    size_t len = len1;
    int mismatch_10 = 0;
    int mismatch_01 = 0;

    for (size_t i = 0; i < len; ++i) {
        if (s1[i] == '1' && s2[i] == '0') {
            mismatch_10++;
        } else if (s1[i] == '0' && s2[i] == '1') {
            mismatch_01++;
        }
    }

    if ((mismatch_10 + mismatch_01) % 2 != 0) {
        return -1;
    }

    return (mismatch_10 / 2) + (mismatch_01 / 2) + (mismatch_10 % 2) + (mismatch_01 % 2);
}

int main(int argc, char *argv[]) {
    const char *s1 = "110011";
    const char *s2 = "010111";

    if (argc == 3) {
        s1 = argv[1];
        s2 = argv[2];
    }

    int result = min_swaps_binary(s1, s2);

    printf("%d\n", result);

    return 0;
}