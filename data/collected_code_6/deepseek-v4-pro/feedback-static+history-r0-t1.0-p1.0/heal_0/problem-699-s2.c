#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int min_swaps_to_convert(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) {
        return -1;
    }

    size_t len = len1;
    int count_s1 = 0;
    int count_s2 = 0;
    int mismatches = 0;
    int ones_mismatch = 0;

    for (size_t i = 0; i < len; i++) {
        if (s1[i] == '1') {
            count_s1++;
        }
        if (s2[i] == '1') {
            count_s2++;
        }
        if (s1[i] != s2[i]) {
            mismatches++;
            if (s1[i] == '1') {
                ones_mismatch++;
            }
        }
    }

    if (count_s1 != count_s2) {
        return -1;
    }

    return ones_mismatch < (mismatches - ones_mismatch) ? ones_mismatch : (mismatches - ones_mismatch);
}

int main(void) {
    char s1[1024];
    char s2[1024];

    printf("Enter first binary string: ");
    if (fgets(s1, sizeof(s1), stdin) == NULL) {
        return 1;
    }
    s1[strcspn(s1, "\n")] = '\0';

    printf("Enter second binary string: ");
    if (fgets(s2, sizeof(s2), stdin) == NULL) {
        return 1;
    }
    s2[strcspn(s2, "\n")] = '\0';

    int result = min_swaps_to_convert(s1, s2);

    if (result == -1) {
        printf("-1\n");
    } else {
        printf("%d\n", result);
    }

    return 0;
}