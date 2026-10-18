#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 4096

static size_t bounded_strlen(const char *str, size_t max_len)
{
    size_t len = 0;

    while (len < max_len && str[len] != '\0') {
        len++;
    }

    return len;
}

void reverse_string(char *str)
{
    size_t left = 0;
    size_t right;

    if (str == NULL) {
        return;
    }

    right = bounded_strlen(str, MAX_STRING_LENGTH);
    if (right == 0) {
        return;
    }
    right--;

    while (left < right) {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

int reverse_string_list(char **list, size_t count)
{
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

int main(void)
{
    char first[] = "hello";
    char second[] = "world";
    char third[] = "programming";
    char fourth[] = "c";
    char fifth[] = "developer";
    char *strings[] = {first, second, third, fourth, fifth};
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