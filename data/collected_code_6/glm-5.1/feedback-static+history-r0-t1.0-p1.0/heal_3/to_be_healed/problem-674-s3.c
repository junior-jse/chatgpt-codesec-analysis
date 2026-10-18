#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *remove_duplicate_words(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strnlen(input, SIZE_MAX);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }

    char **words = (char **)malloc((len / 2 + 1) * sizeof(char *));
    if (words == NULL) {
        free(buffer);
        return NULL;
    }

    size_t word_count = 0;

    char *temp = (char *)malloc(len + 1);
    if (temp == NULL) {
        free(buffer);
        free(words);
        return NULL;
    }
    snprintf(temp, len + 1, "%s", input);

    char *token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        bool found = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            words[word_count] = token;
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    /* Possible weaknesses found:
     *  Either the condition 'buffer!=NULL' is redundant or there is possible null pointer dereference: buffer. [nullPointerRedundantCheck]
     *  Null pointer dereference
     */
    buffer[0] = '\0';
    size_t current_len = 0;
    for (size_t i = 0; i < word_count; i++) {
        size_t word_len = strnlen(words[i], len + 1);
        /* Possible weaknesses found:
         *  Assuming that condition 'current_len+word_len<len+1' is not redundant
         */
        if (current_len + word_len < len + 1) {
            /* Possible weaknesses found:
             *  Condition 'current_len+word_len<=len+1' is always true [knownConditionTrueFalse]
             *  Assuming that condition 'buffer!=NULL' is not redundant
             *  Condition 'current_len+word_len<=len+1' is always true
             */
            if (buffer != NULL && current_len + word_len <= len + 1) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(buffer + current_len, words[i], word_len);
            }
            current_len += word_len;
        }
        if (i < word_count - 1) {
            if (current_len + 1 < len + 1) {
                if (buffer != NULL && current_len + 1 <= len + 1) {
                    buffer[current_len] = ' ';
                }
                current_len++;
            }
        }
    }
    if (current_len <= len) {
        buffer[current_len] = '\0';
    } else {
        buffer[len] = '\0';
    }

    size_t result_len = strnlen(buffer, len + 1);
    char *result = (char *)malloc(result_len + 1);
    if (result != NULL) {
        snprintf(result, result_len + 1, "%s", buffer);
    }

    free(temp);
    free(words);
    free(buffer);

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
     *  Calling function 'remove_duplicate_words' returns 0
     *  Assignment 'null_test=remove_duplicate_words(NULL)', assigned value is 0
     *  Variable 'null_test' can be declared as pointer to const [constVariablePointer]
     */
    char *null_test = remove_duplicate_words(NULL);
    /* Possible weaknesses found:
     *  Condition 'null_test==NULL' is always true
     *  Condition 'null_test==NULL' is always true [knownConditionTrueFalse]
     */
    if (null_test == NULL) {
        printf("NULL input handled safely.\n");
    }

    return 0;
}