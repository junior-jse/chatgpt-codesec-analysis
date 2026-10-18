#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverse_string(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = strlen(str);
    char *rev = (char *)malloc(len + 1);
    if (rev == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        rev[i] = str[len - 1 - i];
    }
    rev[len] = '\0';
    return rev;
}

int count_reverse_pairs(char **strings, int count) {
    if (strings == NULL || count <= 0) {
        return 0;
    }
    int pair_count = 0;
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            char *rev = reverse_string(strings[i]);
            if (rev != NULL) {
                for (int j = i + 1; j < count; j++) {
                    if (strings[j] != NULL && strcmp(strings[i], strings[j]) != 0 && strcmp(rev, strings[j]) == 0) {
                        pair_count++;
                    }
                }
                free(rev);
            }
        }
    }
    return pair_count;
}

int main(void) {
    char *arr[] = {"abc", "cba", "hello", "world", "olleh", "abc", "cba"};
    int count = sizeof(arr) / sizeof(arr[0]);
    int result = count_reverse_pairs(arr, count);
    printf("%d\n", result);
    return 0;
}