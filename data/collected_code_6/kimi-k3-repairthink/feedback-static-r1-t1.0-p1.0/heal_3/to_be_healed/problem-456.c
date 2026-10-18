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

static char *duplicate_string(const char *src, size_t max_len)
{
    size_t len;
    char *copy;

    if (src == NULL || max_len == 0)
    {
        return NULL;
    }

    len = strnlen(src, max_len);
    copy = malloc(len + 1);

    if (copy == NULL)
    {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, src, len);
    copy[len] = '\0';

    return copy;
}

int main(void)
{
    static const char *inputs[] = { "hello", "world", "programming", "language", "secure" };
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char *strings[sizeof(inputs) / sizeof(inputs[0])];
    size_t count;
    size_t i;
    size_t j;
    int result;

    count = sizeof(inputs) / sizeof(inputs[0]);

    for (i = 0; i < count; i++)
    {
        strings[i] = duplicate_string(inputs[i], MAX_STRING_LEN - 1);
        if (strings[i] == NULL)
        {
            for (j = 0; j < i; j++)
            {
                free(strings[j]);
            }
            fprintf(stderr, "Error: failed to allocate string\n");
            return EXIT_FAILURE;
        }
    }

    result = reverse_string_list(strings, count, MAX_STRING_LEN);

    if (result != 0)
    {
        fprintf(stderr, "Error: failed to reverse string list\n");
        for (i = 0; i < count; i++)
        {
            free(strings[i]);
        }
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; i++)
    {
        if (printf("%s\n", strings[i]) < 0)
        {
            fprintf(stderr, "Error: failed to print output\n");
            for (j = 0; j < count; j++)
            {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < count; i++)
    {
        free(strings[i]);
    }

    return EXIT_SUCCESS;
}