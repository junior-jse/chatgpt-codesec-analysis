#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverse_string(const char *str) {
    size_t len = strlen(str);
    char *rev = malloc(len + 1);
    if (!rev) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        rev[i] = str[len - 1 - i];
    }
    rev[len] = '\0';
    return rev;
}

int count_reverse_pairs(const char **list, size_t size) {
    int count = 0;
    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) continue;
        char *rev = reverse_string(list[i]);
        if (rev == NULL) continue;
        for (size_t j = i + 1; j < size; j++) {
            if (list[j] != NULL && strcmp(list[j], rev) == 0) {
                count++;
            }
        }
        free(rev);
    }
    return count;
}

int main(void) {
    const char *list[] = {"abc", "cba", "hello", "world", "olleh", "abc", "cba"};
    size_t size = sizeof(list) / sizeof(list[0]);
    int pairs = count_reverse_pairs(list, size);
    printf("%d\n", pairs);
    return 0;
}