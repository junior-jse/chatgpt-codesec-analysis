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

    WordCount *wordCounts = calloc(MAX_WORDS, sizeof(WordCount));
    if (wordCounts == NULL) {
        return NULL;
    }

    int wordCountSize = 0;

    for (int i = 0; i < stringsCount; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        size_t strLen = strnlen(strings[i], MAX_WORD_LENGTH * MAX_WORDS);
        if (strLen == 0 || strLen >= MAX_WORD_LENGTH * MAX_WORDS) {
            continue;
        }

        char *tempString = malloc(strLen + 1);
        if (tempString == NULL) {
            free(wordCounts);
            return NULL;
        }
        
        memcpy(tempString, strings[i], strLen);
        tempString[strLen] = '\0';

        char *saveptr = NULL;
        char *token = strtok_r(tempString, " \t\n\r", &saveptr);
        
        while (token != NULL) {
            size_t tokenLen = strnlen(token, MAX_WORD_LENGTH);
            
            if (tokenLen > 0 && tokenLen < MAX_WORD_LENGTH) {
                int index = findWordIndex(wordCounts, wordCountSize, token);
                
                if (index != -1) {
                    wordCounts[index].count++;
                } else {
                    if (wordCountSize < MAX_WORDS) {
                        memcpy(wordCounts[wordCountSize].word, token, tokenLen);
                        wordCounts[wordCountSize].word[tokenLen] = '\0';
                        wordCounts[wordCountSize].count = 1;
                        wordCountSize++;
                    }
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

    size_t resultLen = strnlen(wordCounts[maxIndex].word, MAX_WORD_LENGTH);
    char *result = malloc(resultLen + 1);
    
    if (result == NULL) {
        free(wordCounts);
        return NULL;
    }
    
    memcpy(result, wordCounts[maxIndex].word, resultLen);
    result[resultLen] = '\0';

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