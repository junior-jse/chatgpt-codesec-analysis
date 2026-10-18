#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000
#define MAX_STRING_LENGTH 65536

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int findWordIndex(const WordCount *wordCounts, int wordCountSize, const char *word) {
    if (wordCounts == NULL || word == NULL || wordCountSize <= 0) {
        return -1;
    }
    for (int i = 0; i < wordCountSize; i++) {
        if (strncmp(wordCounts[i].word, word, MAX_WORD_LENGTH) == 0) {
            return i;
        }
    }
    return -1;
}

char *getMostFrequentWord(const char **strings, int stringsCount) {
    if (strings == NULL || stringsCount <= 0) {
        return NULL;
    }

    WordCount *wordCounts = calloc(MAX_WORDS, sizeof(WordCount));
    if (wordCounts == NULL) {
        return NULL;
    }

    int wordCountSize = 0;

    for (int i = 0; i < stringsCount; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        size_t stringLength = strnlen(strings[i], MAX_STRING_LENGTH);
        if (stringLength >= MAX_STRING_LENGTH) {
            continue;
        }

        char *tempString = malloc(stringLength + 1);
        if (tempString == NULL) {
            free(wordCounts);
            return NULL;
        }

        if (stringLength > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(tempString, strings[i], stringLength);
        }
        tempString[stringLength] = '\0';

        char *saveptr = NULL;
        char *token = strtok_r(tempString, " \t\n\r", &saveptr);
        while (token != NULL) {
            size_t tokenLength = strnlen(token, MAX_WORD_LENGTH);
            if (tokenLength >= MAX_WORD_LENGTH) {
                token = strtok_r(NULL, " \t\n\r", &saveptr);
                continue;
            }

            int index = findWordIndex(wordCounts, wordCountSize, token);
            if (index != -1) {
                if (wordCounts[index].count < __INT_MAX__) {
                    wordCounts[index].count++;
                }
            } else {
                if (wordCountSize < MAX_WORDS &&
                    tokenLength + 1 <= sizeof(wordCounts[wordCountSize].word)) {
                    if (tokenLength > 0) {
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(wordCounts[wordCountSize].word, token, tokenLength);
                    }
                    wordCounts[wordCountSize].word[tokenLength] = '\0';
                    wordCounts[wordCountSize].count = 1;
                    wordCountSize++;
                }
            }
            token = strtok_r(NULL, " \t\n\r", &saveptr);
        }

        free(tempString);
    }

    if (wordCountSize == 0) {
        free(wordCounts);
        return NULL;
    }

    int maxIndex = 0;
    for (int i = 1; i < wordCountSize; i++) {
        if (wordCounts[i].count > wordCounts[maxIndex].count) {
            maxIndex = i;
        }
    }

    size_t resultLength = strnlen(wordCounts[maxIndex].word, MAX_WORD_LENGTH);
    if (resultLength >= MAX_WORD_LENGTH) {
        free(wordCounts);
        return NULL;
    }

    char *result = malloc(resultLength + 1);
    if (result == NULL) {
        free(wordCounts);
        return NULL;
    }
    if (resultLength > 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result, wordCounts[maxIndex].word, resultLength);
    }
    result[resultLength] = '\0';

    free(wordCounts);
    return result;
}

int main(void) {
    const char *strings[] = {
        "hello world hello",
        "world of programming",
        "hello programming world",
        "the quick brown fox",
        "hello hello world"
    };
    int stringsCount = sizeof(strings) / sizeof(strings[0]);

    char *mostFrequent = getMostFrequentWord(strings, stringsCount);

    if (mostFrequent != NULL) {
        printf("Most frequent word: %s\n", mostFrequent);
        free(mostFrequent);
    } else {
        printf("No words found or error occurred.\n");
    }

    return 0;
}