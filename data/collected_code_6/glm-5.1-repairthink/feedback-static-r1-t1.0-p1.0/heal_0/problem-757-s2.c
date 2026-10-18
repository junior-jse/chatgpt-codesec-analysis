#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_reverse(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
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

int count_reverse_pairs(char **strings, size_t size) {
    int count = 0;
    if (strings == NULL || size == 0) {
        return 0;
    }
    for (size_t i = 0; i < size; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        for (size_t j = i + 1; j < size; j++) {
            if (strings[j] == NULL) {
                continue;
            }
            if (is_reverse(strings[i], strings[j])) {
                count++;
            }
        }
    }
    return count;
}

int main(void) {
    char *arr[] = {"abc", "cba", "hello", "olleh", "world", "test"};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int result = count_reverse_pairs(arr, size);
    printf("%d\n", result);
    return 0;
}