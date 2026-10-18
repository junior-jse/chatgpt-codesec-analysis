#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *join_tuple(const char **tuple, size_t count, const char *delimiter)
{
    size_t total_length = 1;
    size_t delimiter_length;
    size_t i;
    char *result;
    char *current;

    if (tuple == NULL || delimiter == NULL)
    {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    delimiter_length = strlen(delimiter);

    for (i = 0; i < count; i++)
    {
        if (tuple[i] == NULL)
        {
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        total_length += strlen(tuple[i]);
        if (i < count - 1)
        {
            total_length += delimiter_length;
        }
    }

    result = (char *)malloc(total_length);
    if (result == NULL)
    {
        return NULL;
    }

    current = result;
    for (i = 0; i < count; i++)
    {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t element_length = strlen(tuple[i]);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(current, tuple[i], element_length);
        current += element_length;

        if (i < count - 1)
        {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(current, delimiter, delimiter_length);
            current += delimiter_length;
        }
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