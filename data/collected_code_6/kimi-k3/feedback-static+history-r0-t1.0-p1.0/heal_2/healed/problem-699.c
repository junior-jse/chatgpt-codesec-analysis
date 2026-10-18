#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int min_swaps(const char *s1, const char *s2, size_t max_len) {
    if (s1 == NULL || s2 == NULL || max_len == 0) {
        return -1;
    }

    size_t len1 = strnlen(s1, max_len);
    size_t len2 = strnlen(s2, max_len);

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
    char *s1 = NULL;
    char *s2 = NULL;
    size_t len1 = 0;
    size_t len2 = 0;
    ssize_t read1;
    ssize_t read2;

    read1 = getline(&s1, &len1, stdin);
    if (read1 == -1) {
        free(s1);
        return 1;
    }

    read2 = getline(&s2, &len2, stdin);
    if (read2 == -1) {
        free(s1);
        free(s2);
        return 1;
    }

    if (read1 > 0 && s1[read1 - 1] == '\n') {
        s1[read1 - 1] = '\0';
    }
    if (read2 > 0 && s2[read2 - 1] == '\n') {
        s2[read2 - 1] = '\0';
    }

    int result = min_swaps(s1, s2, (len1 > len2) ? len1 : len2);
    if (result == -1) {
        printf("Conversion not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }

    free(s1);
    free(s2);

    return 0;
}