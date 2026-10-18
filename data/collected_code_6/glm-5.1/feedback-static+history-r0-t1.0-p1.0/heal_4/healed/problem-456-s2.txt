#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return;
    }
    size_t len = strnlen(str, max_len);
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

void reverse_string_list(char **list, size_t count, size_t max_str_len) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        reverse_string(list[i], max_str_len);
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
    size_t buffer_size = sizeof(list[0]);
    char *ptrs[4];
    for (size_t i = 0; i < count; i++) {
        if (i < sizeof(ptrs) / sizeof(ptrs[0])) {
            ptrs[i] = list[i];
        }
    }
    reverse_string_list(ptrs, count, buffer_size - 1);
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", ptrs[i]);
    }
    return 0;
}