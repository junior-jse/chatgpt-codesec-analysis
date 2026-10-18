#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL)
    {
        return NULL;
    }

    length = strlen(source);
    copy = (char *)malloc(length + 1U);

    if (copy == NULL)
    {
        return NULL;
    }

    (void)memcpy(copy, source, length + 1U);
    return copy;
}

static int is_word_in_list(const char *word, char * const *list, size_t list_size)
{
    size_t i;
    int found = 0;

    if ((word == NULL) || (list == NULL))
    {
        return 0;
    }

    for (i = 0U; i < list_size; i++)
    {
        if ((list[i] != NULL) && (strcmp(word, list[i]) == 0))
        {
            found = 1;
            break;
        }
    }

    return found;
}

static void free_word_list(char **list, size_t count)
{
    size_t i;

    if (list == NULL)
    {
        return;
    }

    for (i = 0U; i < count; i++)
    {
        free(list[i]);
        list[i] = NULL;
    }

    free(list);
}

static char **remove_words(char * const *input_list, size_t input_count,
                           char * const *words_to_remove, size_t remove_count,
                           size_t *output_count)
{
    char **result;
    size_t i;
    size_t result_index = 0U;
    size_t kept_count = 0U;

    if (output_count == NULL)
    {
        return NULL;
    }

    *output_count = 0U;

    if ((input_list == NULL) || (input_count == 0U))
    {
        return NULL;
    }

    if ((remove_count > 0U) && (words_to_remove == NULL))
    {
        return NULL;
    }

    for (i = 0U; i < input_count; i++)
    {
        if ((input_list[i] != NULL) &&
            !is_word_in_list(input_list[i], words_to_remove, remove_count))
        {
            kept_count++;
        }
    }

    if (kept_count == 0U)
    {
        return NULL;
    }

    result = (char **)calloc(kept_count, sizeof(char *));

    if (result == NULL)
    {
        return NULL;
    }

    for (i = 0U; i < input_count; i++)
    {
        if ((input_list[i] != NULL) &&
            !is_word_in_list(input_list[i], words_to_remove, remove_count))
        {
            result[result_index] = duplicate_string(input_list[i]);

            if (result[result_index] == NULL)
            {
                free_word_list(result, kept_count);
                return NULL;
            }

            result_index++;
        }
    }

    *output_count = kept_count;
    return result;
}

int main(void)
{
    char *word_list[] = {"apple", "banana", "cherry", "date", "elderberry"};
    char *remove_list[] = {"banana", "date"};
    char **filtered_list;
    size_t input_count = sizeof(word_list) / sizeof(word_list[0]);
    size_t remove_count = sizeof(remove_list) / sizeof(remove_list[0]);
    size_t output_count = 0U;
    size_t i;

    filtered_list = remove_words(word_list, input_count, remove_list, remove_count, &output_count);

    if ((filtered_list == NULL) && (output_count == 0U) && (input_count > 0U))
    {
        if (remove_count > 0U)
        {
            (void)fprintf(stderr, "Error: Failed to filter word list\n");
            return EXIT_FAILURE;
        }
    }

    if (filtered_list != NULL)
    {
        (void)printf("Filtered words:\n");

        for (i = 0U; i < output_count; i++)
        {
            if (filtered_list[i] != NULL)
            {
                (void)printf("%s\n", filtered_list[i]);
            }
        }

        free_word_list(filtered_list, output_count);
    }
    else
    {
        (void)printf("No words remaining after filtering\n");
    }

    return EXIT_SUCCESS;
}