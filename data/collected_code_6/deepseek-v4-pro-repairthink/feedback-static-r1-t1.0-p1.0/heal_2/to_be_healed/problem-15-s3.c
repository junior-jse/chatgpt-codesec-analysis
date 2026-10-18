#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **split_at_lowercase(const char *str, int *count)
{
    if (str == NULL || count == NULL)
    {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    size_t capacity = 10;
    char **result = malloc(capacity * sizeof(char *));
    if (result == NULL)
    {
        return NULL;
    }

    size_t segment_count = 0;
    size_t segment_start = 0;

    for (size_t i = 0; i <= len; i++)
    {
        if (i == len || islower((unsigned char)str[i]))
        {
            size_t segment_len = i - segment_start;
            if (segment_len > 0)
            {
                if (segment_count >= capacity)
                {
                    capacity *= 2;
                    char **temp = realloc(result, capacity * sizeof(char *));
                    if (temp == NULL)
                    {
                        for (size_t j = 0; j < segment_count; j++)
                        {
                            free(result[j]);
                        }
                        free(result);
                        return NULL;
                    }
                    result = temp;
                }

                result[segment_count] = malloc(segment_len + 1);
                if (result[segment_count] == NULL)
                {
                    for (size_t j = 0; j < segment_count; j++)
                    {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }

                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[segment_count], str + segment_start, segment_len);
                result[segment_count][segment_len] = '\0';
                segment_count++;
            }
            segment_start = i + 1;
        }
    }

    if (segment_count == 0)
    {
        free(result);
        *count = 0;
        return NULL;
    }

    *count = (int)segment_count;
    return result;
}

int main(void)
{
    const char *test_string = "HelloWorldThisIsATest";
    int count = 0;
    char **parts = split_at_lowercase(test_string, &count);

    if (parts == NULL)
    {
        printf("No parts found or error occurred\n");
        return 0;
    }

    printf("Split results:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d: %s\n", i, parts[i]);
        free(parts[i]);
    }
    free(parts);

    return 0;
}