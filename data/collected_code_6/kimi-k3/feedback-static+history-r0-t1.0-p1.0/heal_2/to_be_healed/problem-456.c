#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str)
{
    size_t left;
    size_t right;

    if (str == NULL)
    {
        return;
    }

    left = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    right = strlen(str);

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

int reverse_string_list(char **list, size_t count)
{
    size_t i;

    if (list == NULL)
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
        reverse_string(list[i]);
    }

    return 0;
}

int main(void)
{
    char str1[] = "hello";
    char str2[] = "world";
    char str3[] = "programming";
    char str4[] = "language";
    char str5[] = "secure";
    char *strings[] = {str1, str2, str3, str4, str5};
    size_t count;
    size_t i;
    int result;

    count = sizeof(strings) / sizeof(strings[0]);

    result = reverse_string_list(strings, count);

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