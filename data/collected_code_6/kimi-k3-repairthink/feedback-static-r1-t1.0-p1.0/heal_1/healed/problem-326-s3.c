#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000
#define MAX_STRING_LENGTH 65536

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int findWordIndex(WordCount *wordCounts, int wordCountSize, const char *word) {
    for (int i = 0; i < wordCountSize; i++) {
        if (strncmp(wordCounts[i].word, word, MAX_WORD_LENGTH) == 0) {
            return i;
        }
    }
    return -1;
}

char *getMostFrequentWord(char **strings, int stringsCount) {
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
        memcpy(tempString, strings[i], stringLength + 1);

        char *token = strtok(tempString, " \t\n\r");
        while (token != NULL) {
            size_t tokenLength = strnlen(token, MAX_WORD_LENGTH);
            if (tokenLength >= MAX_WORD_LENGTH) {
                token = strtok(NULL, " \t\n\r");
                continue;
            }

            int index = findWordIndex(wordCounts, wordCountSize, token);
            if (index != -1) {
                wordCounts[index].count++;
            } else {
                if (wordCountSize < MAX_WORDS) {
                    memcpy(wordCounts[wordCountSize].word, token, tokenLength + 1);
                    wordCounts[wordCountSize].count = 1;
                    wordCountSize++;
                }
            }
            token = strtok(NULL, " \t\n\r");
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
    char *result = malloc(resultLength + 1);
    if (result == NULL) {
        free(wordCounts);
        return NULL;
    }
    memcpy(result, wordCounts[maxIndex].word, resultLength + 1);

    free(wordCounts);
    return result;
}

int main(void) {
    char *strings[] = {
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