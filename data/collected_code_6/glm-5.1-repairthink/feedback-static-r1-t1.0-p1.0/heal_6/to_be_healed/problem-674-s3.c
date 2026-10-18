#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    const char *word;
    size_t length;
} WordEntry;

char *remove_duplicate_words(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    size_t max_words = len / 2 + 1;
    
    WordEntry *words = malloc(max_words * sizeof(WordEntry));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    const char *p = input;

    while (*p) {
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        if (!*p) {
            break;
        }

        const char *start = p;
        while (*p && !isspace((unsigned char)*p)) {
            p++;
        }
        size_t token_len = p - start;

        bool found = false;
        for (size_t i = 0; i < word_count; i++) {
            if (words[i].length == token_len && memcmp(words[i].word, start, token_len) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            if (word_count < max_words) {
                words[word_count].word = start;
                words[word_count].length = token_len;
                word_count++;
            }
        }
    }

    size_t result_size = 0;
    for (size_t i = 0; i < word_count; i++) {
        result_size += words[i].length;
    }
    if (word_count > 0) {
        result_size += word_count - 1;
    }

    char *result = malloc(result_size + 1);
    if (result == NULL) {
        free(words);
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (offset + words[i].length <= result_size) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + offset, words[i].word, words[i].length);
        }
        offset += words[i].length;
        if (i < word_count - 1) {
            if (offset < result_size) {
                result[offset] = ' ';
            }
            offset++;
        }
    }
    result[result_size] = '\0';

    free(words);

    return result;
}

int main(void) {
    char str[] = "hello world hello there world again";
    char *cleaned = remove_duplicate_words(str);

    if (cleaned != NULL) {
        printf("Original: %s\n", str);
        printf("Cleaned: %s\n", cleaned);
        free(cleaned);
    }

    /* Possible weaknesses found:
     *  Assignment 'null_test=remove_duplicate_words(NULL)', assigned value is 0
     *  Calling function 'remove_duplicate_words' returns 0
     */
    const char *null_test = remove_duplicate_words(NULL);
    /* Possible weaknesses found:
     *  Condition 'null_test==NULL' is always true [knownConditionTrueFalse]
     *  Condition 'null_test==NULL' is always true
     */
    if (null_test == NULL) {
        printf("NULL input handled safely.\n");
    }

    return 0;
}