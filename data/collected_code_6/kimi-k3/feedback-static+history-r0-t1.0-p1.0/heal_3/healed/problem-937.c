#include <stdio.h>
#include <string.h>
#include <limits.h>

char find_most_common_char(const char *str, int *count)
{
    int freq[UCHAR_MAX + 1] = {0};
    int max_count = 0;
    char most_common = '\0';
    size_t i;

    if (str == NULL || count == NULL)
    {
        return '\0';
    }

    for (i = 0; str[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char)str[i];
        freq[c]++;
    }

    for (i = 0; i <= UCHAR_MAX; i++)
    {
        if (freq[i] > max_count)
        {
            max_count = freq[i];
            most_common = (char)i;
        }
    }

    *count = max_count;
    return most_common;
}

int main(void)
{
    char buffer[1024];
    int count = 0;
    char result;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0')
    {
        return 1;
    }

    result = find_most_common_char(buffer, &count);

    if (count > 0)
    {
        printf("Most common character: '%c'\n", result);
        printf("Occurrences: %d\n", count);
    }

    return 0;
}