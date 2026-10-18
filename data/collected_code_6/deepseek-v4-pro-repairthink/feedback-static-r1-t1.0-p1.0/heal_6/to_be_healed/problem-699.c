#include <stdio.h>
#include <string.h>

size_t safe_strlen(const char *s, size_t maxlen) {
    size_t len = 0;
    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
}

int min_swaps(const char *str1, size_t len1, const char *str2, size_t len2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    if (len1 != len2) {
        return -1;
    }

    int count1 = 0, count2 = 0;
    for (size_t i = 0; i < len1; i++) {
        if (str1[i] == '1') count1++;
        if (str2[i] == '1') count2++;
    }

    if (count1 != count2) {
        return -1;
    }

    int diff = 0;
    for (size_t i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            diff++;
        }
    }

    return diff / 2;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str1[1024], str2[1024];

    printf("Enter first binary string: ");
    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        return 1;
    }
    if (strchr(str1, '\n') == NULL) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char dummy[256];
        while (fgets(dummy, sizeof(dummy), stdin) && !strchr(dummy, '\n'));
    }
    str1[strcspn(str1, "\n")] = '\0';

    printf("Enter second binary string: ");
    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        return 1;
    }
    if (strchr(str2, '\n') == NULL) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char dummy[256];
        while (fgets(dummy, sizeof(dummy), stdin) && !strchr(dummy, '\n'));
    }
    str2[strcspn(str2, "\n")] = '\0';

    size_t len1 = safe_strlen(str1, sizeof(str1) - 1);
    size_t len2 = safe_strlen(str2, sizeof(str2) - 1);

    int result = min_swaps(str1, len1, str2, len2);

    if (result == -1) {
        printf("Conversion is not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }

    return 0;
}