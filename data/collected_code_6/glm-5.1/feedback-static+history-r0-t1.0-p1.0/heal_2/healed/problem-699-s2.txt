#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min_swaps(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) {
        return -1;
    }

    size_t count1 = 0;
    size_t count2 = 0;

    for (size_t i = 0; i < len1; ++i) {
        if (s1[i] != '0' && s1[i] != '1') {
            return -1;
        }
        if (s2[i] != '0' && s2[i] != '1') {
            return -1;
        }
        if (s1[i] == '1') {
            ++count1;
        }
        if (s2[i] == '1') {
            ++count2;
        }
    }

    if (count1 != count2) {
        return -1;
    }

    size_t mismatch_1_0 = 0;
    size_t mismatch_0_1 = 0;

    for (size_t i = 0; i < len1; ++i) {
        if (s1[i] == '1' && s2[i] == '0') {
            ++mismatch_1_0;
        } else if (s1[i] == '0' && s2[i] == '1') {
            ++mismatch_0_1;
        }
    }

    if (mismatch_1_0 != mismatch_0_1) {
        return -1;
    }

    return (int)(mismatch_1_0);
}

int main(int argc, char * const argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <binary_string_1> <binary_string_2>\n", argv[0]);
        return 1;
    }

    int result = min_swaps(argv[1], argv[2]);
    if (result == -1) {
        fprintf(stderr, "Invalid input or unequal number of set bits\n");
        return 1;
    }

    printf("%d\n", result);
    return 0;
}