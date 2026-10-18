#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

int isReverse(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return 0;
    }

    size_t len1 = strnlen(s1, MAX_STRING_LENGTH);
    size_t len2 = strnlen(s2, MAX_STRING_LENGTH);

    if (len1 != len2) {
        return 0;
    }

    for (size_t i = 0; i < len1; i++) {
        if (s1[i] != s2[len1 - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

int countReversePairs(const char *const *strings, size_t count) {
    int pairCount = 0;

    if (strings == NULL) {
        return 0;
    }

    for (size_t i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        for (size_t j = i + 1; j < count; j++) {
            if (strings[j] == NULL) {
                continue;
            }
            if (isReverse(strings[i], strings[j])) {
                pairCount++;
            }
        }
    }

    return pairCount;
}

int main(void) {
    const char *testStrings[] = {
        "hello",
        "olleh",
        "world",
        "dlrow",
        "abc",
        "xyz",
        "cba"
    };
    size_t count = sizeof(testStrings) / sizeof(testStrings[0]);

    int result = countReversePairs(testStrings, count);
    printf("Number of reverse pairs: %d\n", result);

    return 0;
}