#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str)
{
    size_t left;
    size_t right;
    char temp;

    if (str == NULL)
    {
        return;
    }

    left = 0;
    right = strlen(str);

    if (right == 0)
    {
        return;
    }

    right--;

    while (left < right)
    {
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
    char *strings[] = {
        "hello",
        "world",
        "programming",
        "language",
        "secure"
    };
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