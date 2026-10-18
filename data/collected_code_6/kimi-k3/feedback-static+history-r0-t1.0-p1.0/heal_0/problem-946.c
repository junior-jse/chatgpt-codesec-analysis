#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_SIZE 10000
#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int findMostCommonElements(const char *text, WordCount *wordCounts, int maxWords) {
    if (text == NULL || wordCounts == NULL || maxWords <= 0) {
        return -1;
    }
    
    int textLen = strlen(text);
    if (textLen == 0 || textLen >= MAX_TEXT_SIZE) {
        return -1;
    }
    
    char *textCopy = (char *)malloc((textLen + 1) * sizeof(char));
    if (textCopy == NULL) {
        return -1;
    }
    
    strcpy(textCopy, text);
    
    int wordCount = 0;
    char *token = strtok(textCopy, " \t\n\r.,;:!?\"'()[]{}");
    
    while (token != NULL && wordCount < maxWords) {
        for (int i = 0; token[i]; i++) {
            token[i] = tolower((unsigned char)token[i]);
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
            strncpy(wordCounts[wordCount].word, token, MAX_WORD_LENGTH - 1);
            wordCounts[wordCount].word[MAX_WORD_LENGTH - 1] = '\0';
            wordCounts[wordCount].count = 1;
            wordCount++;
        }
        
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
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
    char text[MAX_TEXT_SIZE];
    WordCount wordCounts[MAX_WORDS];
    
    printf("Enter text: ");
    if (fgets(text, MAX_TEXT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(text);
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