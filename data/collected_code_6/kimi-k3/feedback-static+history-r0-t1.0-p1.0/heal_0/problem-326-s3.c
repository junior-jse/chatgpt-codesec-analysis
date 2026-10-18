#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int findWordIndex(WordCount *wordCounts, int wordCountSize, const char *word) {
    for (int i = 0; i < wordCountSize; i++) {
        if (strcmp(wordCounts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

char *getMostFrequentWord(char **strings, int stringsCount) {
    if (strings == NULL || stringsCount <= 0) {
        return NULL;
    }

    WordCount *wordCounts = malloc(sizeof(WordCount) * MAX_WORDS);
    if (wordCounts == NULL) {
        return NULL;
    }

    int wordCountSize = 0;

    for (int i = 0; i < stringsCount; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        char *tempString = malloc(strlen(strings[i]) + 1);
        if (tempString == NULL) {
            free(wordCounts);
            return NULL;
        }
        strcpy(tempString, strings[i]);

        char *token = strtok(tempString, " \t\n\r");
        while (token != NULL) {
            if (strlen(token) >= MAX_WORD_LENGTH) {
                token = strtok(NULL, " \t\n\r");
                continue;
            }

            int index = findWordIndex(wordCounts, wordCountSize, token);
            if (index != -1) {
                wordCounts[index].count++;
            } else {
                if (wordCountSize < MAX_WORDS) {
                    strcpy(wordCounts[wordCountSize].word, token);
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

    char *result = malloc(strlen(wordCounts[maxIndex].word) + 1);
    if (result == NULL) {
        free(wordCounts);
        return NULL;
    }
    strcpy(result, wordCounts[maxIndex].word);

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