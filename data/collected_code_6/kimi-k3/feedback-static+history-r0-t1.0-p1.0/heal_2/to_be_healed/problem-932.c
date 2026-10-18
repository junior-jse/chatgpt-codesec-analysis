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

static size_t safe_strlen(const char *s, size_t max_len)
{
    size_t i;
    if (s == NULL) {
        return 0;
    }
    for (i = 0; i < max_len; i++) {
        if (s[i] == '\0') {
            return i;
        }
    }
    return max_len;
}

char **remove_duplicate_words(char **words, int count, int *new_count)
{
    char **result;
    int unique_count;
    int i;
    size_t len;
    size_t max_len = 4096;

    if (words == NULL || count < 0 || new_count == NULL) {
        return NULL;
    }

    *new_count = 0;

    if (count == 0) {
        return NULL;
    }

    result = (char **)calloc((size_t)count, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    unique_count = 0;

    for (i = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }

        len = safe_strlen(words[i], max_len);
        if (len >= max_len) {
            continue;
        }

        if (!is_duplicate(result, unique_count, words[i])) {
            result[unique_count] = (char *)malloc(len + 1);
            if (result[unique_count] == NULL) {
                int j;
                for (j = 0; j < unique_count; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            snprintf(result[unique_count], len + 1, "%s", words[i]);
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
     *  Assignment 'count=sizeof(word_list)/sizeof(word_list[0])', assigned value is 8
     */
    size_t count = sizeof(word_list) / sizeof(word_list[0]);
    int new_count = 0;
    char **unique_words;
    size_t i;

    unique_words = remove_duplicate_words(word_list, (int)count, &new_count);

    /* Possible weaknesses found:
     *  Condition 'count>0' is always true [knownConditionTrueFalse]
     *  Condition 'count>0' is always true
     */
    if (unique_words == NULL && count > 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Original list (%zu words):\n", count);
    for (i = 0; i < count; i++) {
        printf("%s ", word_list[i]);
    }
    printf("\n\n");

    printf("List after removing duplicates (%d words):\n", new_count);
    for (i = 0; i < (size_t)new_count; i++) {
        printf("%s ", unique_words[i]);
    }
    printf("\n");

    free_word_list(unique_words, new_count);

    return EXIT_SUCCESS;
}