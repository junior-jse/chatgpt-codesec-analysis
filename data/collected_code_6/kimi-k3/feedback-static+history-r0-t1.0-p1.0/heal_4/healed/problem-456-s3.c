#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reverse_string(char *str, size_t max_len) {
    size_t left = 0;
    size_t right;
    size_t len = 0;

    if (str == NULL) {
        return -1;
    }

    while (len < max_len && str[len] != '\0') {
        len++;
    }

    if (len == 0) {
        return 0;
    }

    right = len - 1;

    while (left < right) {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }

    return 0;
}

int reverse_string_list(char **list, size_t count, size_t max_len) {
    size_t i;

    if (list == NULL) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        if (list[i] != NULL) {
            if (reverse_string(list[i], max_len) != 0) {
                return -1;
            }
        }
    }

    return 0;
}

int main(void) {
    char *strings[] = {"hello", "world", "programming", "c", "developer"};
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t i;
    int result;
    size_t max_len = 64;

    printf("Original strings:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    result = reverse_string_list(strings, count, max_len);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to reverse string list\n");
        return EXIT_FAILURE;
    }

    printf("\nReversed strings:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    return EXIT_SUCCESS;
}