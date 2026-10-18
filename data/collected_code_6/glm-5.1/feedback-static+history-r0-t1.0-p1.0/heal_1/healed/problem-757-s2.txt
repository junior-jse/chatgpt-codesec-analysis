#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_reverse(const char *s1, const char *s2, size_t max_len) {
    size_t len1 = strnlen(s1, max_len);
    size_t len2 = strnlen(s2, max_len);
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

int count_reverse_pairs(char **strings, size_t size, size_t max_len) {
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
            if (is_reverse(strings[i], strings[j], max_len)) {
                count++;
            }
        }
    }
    return count;
}

int main(void) {
    char *arr[] = {"abc", "cba", "hello", "olleh", "world", "test"};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    size_t max_len = 1024;
    int result = count_reverse_pairs(arr, size, max_len);
    printf("%d\n", result);
    return 0;
}