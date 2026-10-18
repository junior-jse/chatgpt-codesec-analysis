#include <stdio.h>
#include <stdlib.h>
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

static char *read_line(FILE *stream)
{
    size_t capacity = 128;
    size_t length = 0;
    char *buffer = malloc(capacity);
    int ch;

    if (buffer == NULL)
    {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((ch = fgetc(stream)) != EOF && ch != '\n')
    {
        if (length + 1 >= capacity)
        {
            size_t new_capacity = capacity * 2;
            char *new_buffer = realloc(buffer, new_capacity);

            if (new_buffer == NULL)
            {
                free(buffer);
                return NULL;
            }

            buffer = new_buffer;
            capacity = new_capacity;
        }

        buffer[length++] = (char)ch;
    }

    if (ferror(stream) || (ch == EOF && length == 0))
    {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void)
{
    char *buffer;
    int count = 0;
    char result;

    buffer = read_line(stdin);

    if (buffer == NULL)
    {
        return 1;
    }

    if (buffer[0] == '\0')
    {
        free(buffer);
        return 1;
    }

    result = find_most_common_char(buffer, &count);

    if (count > 0)
    {
        printf("Most common character: '%c'\n", result);
        printf("Occurrences: %d\n", count);
    }

    free(buffer);
    return 0;
}