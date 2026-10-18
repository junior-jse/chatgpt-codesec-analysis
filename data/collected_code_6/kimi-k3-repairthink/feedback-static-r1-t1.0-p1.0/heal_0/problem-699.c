#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int min_swaps(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) {
        return -1;
    }

    int diff = 0;
    for (size_t i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) {
            diff++;
        }
    }

    if (diff % 2 != 0) {
        return -1;
    }

    return diff / 2;
}

int main(void) {
    char s1[100];
    char s2[100];

    if (fgets(s1, sizeof(s1), stdin) == NULL) {
        return 1;
    }
    if (fgets(s2, sizeof(s2), stdin) == NULL) {
        return 1;
    }

    s1[strcspn(s1, "\n")] = '\0';
    s2[strcspn(s2, "\n")] = '\0';

    int result = min_swaps(s1, s2);
    if (result == -1) {
        printf("Conversion not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }

    return 0;
}