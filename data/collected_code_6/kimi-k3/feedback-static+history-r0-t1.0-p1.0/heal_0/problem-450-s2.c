#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(const char **list, size_t count, size_t str_len, size_t *out_count)
{
    char **result;
    size_t i;
    size_t valid_count = 0;

    if (list == NULL || out_count == NULL || count == 0)
    {
        return NULL;
    }

    *out_count = 0;

    result = (char **)malloc(count * sizeof(char *));
    if (result == NULL)
    {
        return NULL;
    }

    for (i = 0; i < count; i++)
    {
        if (list[i] != NULL && strlen(list[i]) == str_len)
        {
            result[valid_count] = (char *)malloc((str_len + 1) * sizeof(char));
            if (result[valid_count] == NULL)
            {
                size_t j;
                for (j = 0; j < valid_count; j++)
                {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[valid_count], list[i]);
            valid_count++;
        }
    }

    if (valid_count == 0)
    {
        free(result);
        return NULL;
    }

    *out_count = valid_count;
    return result;
}

void free_string_list(char **list, size_t count)
{
    size_t i;

    if (list == NULL)
    {
        return;
    }

    for (i = 0; i < count; i++)
    {
        free(list[i]);
    }
    free(list);
}

int main(void)
{
    const char *strings[] = {"apple", "hi", "banana", "go", "cherry", "ok", "fig"};
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t target_len = 2;
    size_t extracted_count = 0;
    char **extracted;
    size_t i;

    extracted = extract_strings(strings, count, target_len, &extracted_count);

    if (extracted == NULL)
    {
        printf("No strings of length %zu found or allocation failed.\n", target_len);
        return 1;
    }

    printf("Extracted strings of length %zu:\n", target_len);
    for (i = 0; i < extracted_count; i++)
    {
        printf("%s\n", extracted[i]);
    }

    free_string_list(extracted, extracted_count);

    return 0;
}