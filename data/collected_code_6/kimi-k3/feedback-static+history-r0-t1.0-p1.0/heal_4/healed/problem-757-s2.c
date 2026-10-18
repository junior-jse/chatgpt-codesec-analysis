#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isReverse(const char *s1, size_t len1, const char *s2, size_t len2) {
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

int countReversePairs(char **strings, const size_t *lengths, size_t count) {
    int pairCount = 0;
    
    if (strings == NULL || lengths == NULL) {
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
            if (isReverse(strings[i], lengths[i], strings[j], lengths[j])) {
                pairCount++;
            }
        }
    }
    
    return pairCount;
}

int main(void) {
    char *testStrings[] = {
        "hello",
        "olleh",
        "world",
        "dlrow",
        "abc",
        "xyz",
        "cba"
    };
    size_t count = sizeof(testStrings) / sizeof(testStrings[0]);
    const size_t lengths[] = {5, 5, 5, 5, 3, 3, 3};
    
    int result = countReversePairs(testStrings, lengths, count);
    printf("Number of reverse pairs: %d\n", result);
    
    return 0;
}