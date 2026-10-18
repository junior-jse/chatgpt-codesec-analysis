#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_duplicate(char **words, int count, const char *word)
{
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char **remove_duplicate_words(char **words, int count, int *new_count)
{
    char **result;
    int unique_count;
    int i;

    if (words == NULL || count < 0 || new_count == NULL) {
        return NULL;
    }

    *new_count = 0;

    if (count == 0) {
        return NULL;
    }

    result = (char **)malloc((size_t)count * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    unique_count = 0;

    for (i = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }

        if (!is_duplicate(result, unique_count, words[i])) {
            /* Possible weaknesses found:
             * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            result[unique_count] = (char *)malloc(strlen(words[i]) + 1);
            if (result[unique_count] == NULL) {
                int j;
                for (j = 0; j < unique_count; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(result[unique_count], words[i]);
            unique_count++;
        }
    }

    *new_count = unique_count;
    return result;
}

void free_word_list(char **words, int count)
{
    int i;
    if (words == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

int main(void)
{
    char *word_list[] = {
        "apple",
        "banana",
        "cherry",
        "apple",
        "date",
        "banana",
        "elderberry",
        "cherry"
    };
    /* Possible weaknesses found:
     *  Assignment 'count=(int)(sizeof(word_list)/sizeof(word_list[0]))', assigned value is 8
     */
    int count = (int)(sizeof(word_list) / sizeof(word_list[0]));
    int new_count = 0;
    char **unique_words;
    int i;

    unique_words = remove_duplicate_words(word_list, count, &new_count);

    /* Possible weaknesses found:
     *  Condition 'count>0' is always true [knownConditionTrueFalse]
     *  Condition 'count>0' is always true
     */
    if (unique_words == NULL && new_count == 0 && count > 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Original list (%d words):\n", count);
    for (i = 0; i < count; i++) {
        printf("%s ", word_list[i]);
    }
    printf("\n\n");

    printf("List after removing duplicates (%d words):\n", new_count);
    for (i = 0; i < new_count; i++) {
        printf("%s ", unique_words[i]);
    }
    printf("\n");

    free_word_list(unique_words, new_count);

    return EXIT_SUCCESS;
}