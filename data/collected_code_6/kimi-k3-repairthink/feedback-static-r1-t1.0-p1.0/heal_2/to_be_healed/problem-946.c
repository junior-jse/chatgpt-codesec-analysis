#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_SIZE 10000
#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int findMostCommonElements(const char *text, WordCount *wordCounts, int maxWords) {
    if (text == NULL || wordCounts == NULL || maxWords <= 0) {
        return -1;
    }

    size_t textLen = strnlen(text, MAX_TEXT_SIZE);
    if (textLen == 0 || textLen >= MAX_TEXT_SIZE) {
        return -1;
    }

    char *textCopy = (char *)malloc(textLen + 1);
    if (textCopy == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(textCopy, text, textLen);
    textCopy[textLen] = '\0';

    int wordCount = 0;
    const char *delimiters = " \t\n\r.,;:!?\"'()[]{}";
    char *token = strtok(textCopy, delimiters);

    while (token != NULL && wordCount < maxWords) {
        for (size_t i = 0; token[i] != '\0'; i++) {
            token[i] = (char)tolower((unsigned char)token[i]);
        }

        int found = 0;
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(wordCounts[i].word, token) == 0) {
                wordCounts[i].count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t tokenLen = strlen(token);
            if (tokenLen >= MAX_WORD_LENGTH) {
                tokenLen = MAX_WORD_LENGTH - 1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(wordCounts[wordCount].word, token, tokenLen);
            wordCounts[wordCount].word[tokenLen] = '\0';
            wordCounts[wordCount].count = 1;
            wordCount++;
        }

        token = strtok(NULL, delimiters);
    }

    free(textCopy);
    return wordCount;
}

void sortByFrequency(WordCount *wordCounts, int count) {
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

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char text[MAX_TEXT_SIZE];
    WordCount wordCounts[MAX_WORDS];

    printf("Enter text: ");
    if (fgets(text, MAX_TEXT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
        return 1;
    }

    if (count == 0) {
        printf("No words found in the text\n");
        return 0;
    }

    sortByFrequency(wordCounts, count);

    printf("\nMost common elements and their counts:\n");
    printf("--------------------------------------\n");

    int maxDisplay = (count < 10) ? count : 10;
    for (int i = 0; i < maxDisplay; i++) {
        printf("%d. %s: %d\n", i + 1, wordCounts[i].word, wordCounts[i].count);
    }

    return 0;
}