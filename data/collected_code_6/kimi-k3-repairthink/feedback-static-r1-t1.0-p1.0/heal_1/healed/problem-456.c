#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 256

void reverse_string(char *str, size_t max_len)
{
    size_t left;
    size_t right;

    if (str == NULL || max_len == 0)
    {
        return;
    }

    left = 0;
    right = strnlen(str, max_len);

    if (right == 0)
    {
        return;
    }

    right--;

    while (left < right)
    {
        char temp;

        temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

int reverse_string_list(char **list, size_t count, size_t max_len)
{
    size_t i;

    if (list == NULL || max_len == 0)
    {
        return -1;
    }

    for (i = 0; i < count; i++)
    {
        if (list[i] == NULL)
        {
            return -1;
        }
    }

    for (i = 0; i < count; i++)
    {
        reverse_string(list[i], max_len);
    }

    return 0;
}

int main(void)
{
    char s0[MAX_STRING_LEN] = "hello";
    char s1[MAX_STRING_LEN] = "world";
    char s2[MAX_STRING_LEN] = "programming";
    char s3[MAX_STRING_LEN] = "language";
    char s4[MAX_STRING_LEN] = "secure";
    char *strings[] = { s0, s1, s2, s3, s4 };
    size_t count;
    size_t i;
    int result;

    count = sizeof(strings) / sizeof(strings[0]);

    result = reverse_string_list(strings, count, MAX_STRING_LEN);

    if (result != 0)
    {
        fprintf(stderr, "Error: failed to reverse string list\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; i++)
    {
        if (printf("%s\n", strings[i]) < 0)
        {
            fprintf(stderr, "Error: failed to print output\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}