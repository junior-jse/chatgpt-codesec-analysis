#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
    size_t left = 0;
    size_t right;
    char temp;

    if (str == NULL) {
        return;
    }

    right = strlen(str);
    if (right == 0) {
        return;
    }
    right--;

    while (left < right) {
        temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

int reverse_string_list(char **list, size_t count) {
    size_t i;

    if (list == NULL) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        if (list[i] != NULL) {
            reverse_string(list[i]);
        }
    }

    return 0;
}

int main(void) {
    char *strings[] = {"hello", "world", "programming", "c", "developer"};
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t i;
    int result;

    printf("Original strings:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    result = reverse_string_list(strings, count);
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