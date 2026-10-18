#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_ELEMENT_LENGTH 1024
#define MAX_DELIMITER_LENGTH 64

static int append_bytes(char **cursor, size_t *remaining, const char *source, size_t length)
{
    size_t i;

    if (cursor == NULL || *cursor == NULL || remaining == NULL || source == NULL)
    {
        return -1;
    }

    if (length >= *remaining)
    {
        return -1;
    }

    for (i = 0; i < length; i++)
    {
        (*cursor)[i] = source[i];
    }

    *cursor += length;
    *remaining -= length;

    return 0;
}

char *join_tuple(const char **tuple, size_t count, const char *delimiter)
{
    size_t total_length = 1;
    size_t delimiter_length;
    size_t i;
    size_t remaining;
    char *result;
    char *current;

    if (tuple == NULL || delimiter == NULL)
    {
        return NULL;
    }

    delimiter_length = strnlen(delimiter, MAX_DELIMITER_LENGTH);

    for (i = 0; i < count; i++)
    {
        size_t element_length;

        if (tuple[i] == NULL)
        {
            return NULL;
        }

        element_length = strnlen(tuple[i], MAX_ELEMENT_LENGTH);

        if (element_length > SIZE_MAX - total_length)
        {
            return NULL;
        }
        total_length += element_length;

        if (i + 1 < count)
        {
            if (delimiter_length > SIZE_MAX - total_length)
            {
                return NULL;
            }
            total_length += delimiter_length;
        }
    }

    result = (char *)malloc(total_length);
    if (result == NULL)
    {
        return NULL;
    }

    current = result;
    remaining = total_length;

    for (i = 0; i < count; i++)
    {
        size_t element_length = strnlen(tuple[i], MAX_ELEMENT_LENGTH);

        if (append_bytes(&current, &remaining, tuple[i], element_length) != 0)
        {
            free(result);
            return NULL;
        }

        if (i + 1 < count)
        {
            if (append_bytes(&current, &remaining, delimiter, delimiter_length) != 0)
            {
                free(result);
                return NULL;
            }
        }
    }

    if (remaining < 1)
    {
        free(result);
        return NULL;
    }

    *current = '\0';

    return result;
}

int main(void)
{
    const char *tuple[] = {"apple", "banana", "cherry", "date"};
    size_t count = sizeof(tuple) / sizeof(tuple[0]);
    const char *delimiter = ", ";
    char *joined;

    joined = join_tuple(tuple, count, delimiter);
    if (joined == NULL)
    {
        fprintf(stderr, "Error: failed to join tuple elements\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", joined);

    free(joined);
    joined = NULL;

    return EXIT_SUCCESS;
}