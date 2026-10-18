#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
    if (str == NULL) {
        return;
    }
    size_t len = strnlen(str, SIZE_MAX);
    if (len <= 1) {
        return;
    }
    size_t left = 0;
    size_t right = len - 1;
    while (left < right) {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

void reverse_string_list(char **list, size_t count) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        reverse_string(list[i]);
    }
}

int main(void) {
    char list[][32] = {
        "hello",
        "world",
        "C",
        "programming"
    };
    size_t count = sizeof(list) / sizeof(list[0]);
    char *ptrs[sizeof(list) / sizeof(list[0])];
    for (size_t i = 0; i < count; i++) {
        ptrs[i] = list[i];
    }
    reverse_string_list(ptrs, count);
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", ptrs[i]);
    }
    return 0;
}