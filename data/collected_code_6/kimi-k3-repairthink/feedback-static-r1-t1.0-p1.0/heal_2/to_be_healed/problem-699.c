#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100

int min_swaps(const char *s1, const char *s2, size_t max_len) {
    if (s1 == NULL || s2 == NULL || max_len == 0) {
        return -1;
    }

    size_t len1 = strnlen(s1, max_len);
    size_t len2 = strnlen(s2, max_len);

    if (len1 >= max_len || len2 >= max_len) {
        return -1;
    }

    if (len1 != len2) {
        return -1;
    }

    size_t diff = 0;
    for (size_t i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) {
            diff++;
        }
    }

    if (diff % 2 != 0) {
        return -1;
    }

    return (int)(diff / 2);
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char s1[MAX_LEN] = {0};
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char s2[MAX_LEN] = {0};

    if (fgets(s1, sizeof(s1), stdin) == NULL) {
        return 1;
    }
    if (fgets(s2, sizeof(s2), stdin) == NULL) {
        return 1;
    }

    s1[sizeof(s1) - 1] = '\0';
    s2[sizeof(s2) - 1] = '\0';

    size_t n1 = strcspn(s1, "\n");
    size_t n2 = strcspn(s2, "\n");
    if (n1 < sizeof(s1)) {
        s1[n1] = '\0';
    }
    if (n2 < sizeof(s2)) {
        s2[n2] = '\0';
    }

    int result = min_swaps(s1, s2, sizeof(s1));
    if (result == -1) {
        printf("Conversion not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }

    return 0;
}