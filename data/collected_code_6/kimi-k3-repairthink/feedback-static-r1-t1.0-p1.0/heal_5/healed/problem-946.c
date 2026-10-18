#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_SIZE 10000
#define MAX_WORDS 1000

typedef struct {
    char *word;
    int count;
} WordCount;

static int safeStringCopy(char *dest, size_t destSize, const char *src, size_t srcLen)
{
    size_t i;

    if (dest == NULL || src == NULL || destSize == 0) {
        return -1;
    }

    if (srcLen >= destSize) {
        return -1;
    }

    for (i = 0; i < srcLen && i < destSize - 1; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';

    return 0;
}

static char *normalizeToken(const char *token)
{
    size_t i;
    size_t tokenLen;
    char *normalized;

    if (token == NULL) {
        return NULL;
    }

    tokenLen = strlen(token);
    if (tokenLen == 0) {
        return NULL;
    }

    normalized = (char *)malloc(tokenLen + 1);
    if (normalized == NULL) {
        return NULL;
    }

    for (i = 0; i < tokenLen; i++) {
        normalized[i] = (char)tolower((unsigned char)token[i]);
    }
    normalized[tokenLen] = '\0';

    return normalized;
}

static void freeWordEntries(WordCount *wordCounts, int count)
{
    int i;

    if (wordCounts == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        free(wordCounts[i].word);
        wordCounts[i].word = NULL;
    }
}

int findMostCommonElements(const char *text, WordCount *wordCounts, int maxWords)
{
    if (text == NULL || wordCounts == NULL || maxWords <= 0) {
        return -1;
    }

    size_t textLen = strnlen(text, MAX_TEXT_SIZE);
    if (textLen == 0 || textLen >= MAX_TEXT_SIZE) {
        return -1;
    }

    size_t copyCapacity = textLen + 1;
    char *textCopy = (char *)malloc(copyCapacity);
    if (textCopy == NULL) {
        return -1;
    }

    if (safeStringCopy(textCopy, copyCapacity, text, textLen) != 0) {
        free(textCopy);
        return -1;
    }

    int wordCount = 0;
    const char *delimiters = " \t\n\r.,;:!?\"'()[]{}";
    char *token = strtok(textCopy, delimiters);

    while (token != NULL) {
        char *normalized = normalizeToken(token);

        if (normalized == NULL) {
            token = strtok(NULL, delimiters);
            continue;
        }

        int found = 0;
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(wordCounts[i].word, normalized) == 0) {
                wordCounts[i].count++;
                found = 1;
                break;
            }
        }

        if (found) {
            free(normalized);
        } else {
            if (wordCount >= maxWords) {
                free(normalized);
                break;
            }
            wordCounts[wordCount].word = normalized;
            wordCounts[wordCount].count = 1;
            wordCount++;
        }

        token = strtok(NULL, delimiters);
    }

    free(textCopy);
    return wordCount;
}

void sortByFrequency(WordCount *wordCounts, int count)
{
    if (wordCounts == NULL || count <= 0) {
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (wordCounts[j].count < wordCounts[j + 1].count) {
                WordCount temp = wordCounts[j];
                wordCounts[j] = wordCounts[j + 1];
                wordCounts[j + 1] = temp;
            }
        }
    }
}

int main(void)
{
    char *text = (char *)malloc(MAX_TEXT_SIZE);
    if (text == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return 1;
    }

    WordCount *wordCounts = (WordCount *)calloc(MAX_WORDS, sizeof(WordCount));
    if (wordCounts == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        free(text);
        return 1;
    }

    printf("Enter text: ");
    if (fgets(text, MAX_TEXT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(wordCounts);
        free(text);
        return 1;
    }

    text[MAX_TEXT_SIZE - 1] = '\0';

    size_t len = strnlen(text, MAX_TEXT_SIZE);
    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }

    int count = findMostCommonElements(text, wordCounts, MAX_WORDS);

    if (count < 0) {
        fprintf(stderr, "Error processing text\n");
        free(wordCounts);
        free(text);
        return 1;
    }

    if (count == 0) {
        printf("No words found in the text\n");
        free(wordCounts);
        free(text);
        return 0;
    }

    sortByFrequency(wordCounts, count);

    printf("\nMost common elements and their counts:\n");
    printf("--------------------------------------\n");

    int maxDisplay = (count < 10) ? count : 10;
    for (int i = 0; i < maxDisplay; i++) {
        printf("%d. %s: %d\n", i + 1, wordCounts[i].word, wordCounts[i].count);
    }

    freeWordEntries(wordCounts, count);
    free(wordCounts);
    free(text);
    return 0;
}