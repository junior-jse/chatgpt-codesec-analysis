#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000
#define MAX_STRING_LENGTH 65536

typedef struct {
    char *word;
    int count;
} WordCount;

static int copyStringSafely(char *destination, size_t destinationSize, const char *source) {
    size_t length;
    size_t i;

    if (destination == NULL || source == NULL || destinationSize == 0) {
        return -1;
    }

    length = strnlen(source, destinationSize);
    if (length >= destinationSize) {
        destination[0] = '\0';
        return -1;
    }

    for (i = 0; i < length; i++) {
        destination[i] = source[i];
    }
    destination[length] = '\0';

    return 0;
}

static char *duplicateStringBounded(const char *source, size_t maxLength) {
    size_t length;
    char *duplicate;

    if (source == NULL || maxLength == 0) {
        return NULL;
    }

    length = strnlen(source, maxLength);
    if (length >= maxLength) {
        return NULL;
    }

    duplicate = malloc(length + 1);
    if (duplicate == NULL) {
        return NULL;
    }

    if (copyStringSafely(duplicate, length + 1, source) != 0) {
        free(duplicate);
        return NULL;
    }

    return duplicate;
}

static void freeWordCounts(WordCount *wordCounts, int wordCountSize) {
    if (wordCounts == NULL) {
        return;
    }
    for (int i = 0; i < wordCountSize; i++) {
        free(wordCounts[i].word);
    }
    free(wordCounts);
}

int findWordIndex(const WordCount *wordCounts, int wordCountSize, const char *word) {
    if (wordCounts == NULL || word == NULL || wordCountSize <= 0) {
        return -1;
    }
    for (int i = 0; i < wordCountSize; i++) {
        if (wordCounts[i].word != NULL && strcmp(wordCounts[i].word, word) == 0) {
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
            freeWordCounts(wordCounts, wordCountSize);
            return NULL;
        }

        if (copyStringSafely(tempString, stringLength + 1, strings[i]) != 0) {
            free(tempString);
            continue;
        }

        char *saveptr = NULL;
        char *token = strtok_r(tempString, " \t\n\r", &saveptr);
        while (token != NULL) {
            size_t tokenLength = strnlen(token, MAX_WORD_LENGTH);
            if (tokenLength == 0 || tokenLength >= MAX_WORD_LENGTH) {
                token = strtok_r(NULL, " \t\n\r", &saveptr);
                continue;
            }

            int index = findWordIndex(wordCounts, wordCountSize, token);
            if (index != -1) {
                if (wordCounts[index].count < INT_MAX) {
                    wordCounts[index].count++;
                }
            } else if (wordCountSize < MAX_WORDS) {
                char *wordCopy = duplicateStringBounded(token, MAX_WORD_LENGTH);
                if (wordCopy == NULL) {
                    free(tempString);
                    freeWordCounts(wordCounts, wordCountSize);
                    return NULL;
                }
                wordCounts[wordCountSize].word = wordCopy;
                wordCounts[wordCountSize].count = 1;
                wordCountSize++;
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

    char *result = duplicateStringBounded(wordCounts[maxIndex].word, MAX_WORD_LENGTH);
    if (result == NULL) {
        freeWordCounts(wordCounts, wordCountSize);
        return NULL;
    }

    freeWordCounts(wordCounts, wordCountSize);
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
    int stringsCount = (int)(sizeof(strings) / sizeof(strings[0]));

    char *mostFrequent = getMostFrequentWord(strings, stringsCount);

    if (mostFrequent != NULL) {
        printf("Most frequent word: %s\n", mostFrequent);
        free(mostFrequent);
    } else {
        printf("No words found or error occurred.\n");
    }

    return 0;
}