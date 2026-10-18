#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str, size_t max_len) {
    if (str == NULL) {
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
    if (list == NULL || count == 0) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            reverse_string(list[i], max_str_len);
        }
    }
}

int main(void) {
    char list[][12] = {
        "hello",
        "world",
        "C",
        "programming"
    };

    size_t count = sizeof(list) / sizeof(list[0]);
    size_t max_str_len = sizeof(list[0]);

    reverse_string_list((char **)list, count, max_str_len);

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    return 0;
}