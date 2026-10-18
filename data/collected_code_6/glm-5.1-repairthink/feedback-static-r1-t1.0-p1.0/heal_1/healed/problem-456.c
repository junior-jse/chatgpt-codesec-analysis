#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str, size_t size) {
    if (str == NULL || size == 0) {
        return;
    }

    size_t len = 0;
    while (len < size && str[len] != '\0') {
        len++;
    }

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

void reverse_string_list(char **list, size_t count, size_t *sizes) {
    if (list == NULL || count == 0 || sizes == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            reverse_string(list[i], sizes[i]);
        }
    }
}

int main(void) {
    char s0[] = "hello";
    char s1[] = "world";
    char s2[] = "C";
    char s3[] = "programming";

    char *list[] = {s0, s1, s2, s3};
    size_t sizes[] = {sizeof(s0), sizeof(s1), sizeof(s2), sizeof(s3)};
    size_t count = sizeof(list) / sizeof(list[0]);

    reverse_string_list(list, count, sizes);

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    return 0;
}