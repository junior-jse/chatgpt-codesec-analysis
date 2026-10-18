#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_UNIQUE_WORDS 1000

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

char *get_most_frequent_word(const char *strings[], int num_strings) {
    WordCount *word_counts;
    int unique_words = 0;
    int i, j, k;
    char *token;
    char *string_copy;
    char *saveptr;
    char *result;
    int max_count;
    int max_index;
    size_t len;
    
    if (strings == NULL || num_strings <= 0) {
        return NULL;
    }
    
    word_counts = calloc(MAX_UNIQUE_WORDS, sizeof(WordCount));
    if (word_counts == NULL) {
        return NULL;
    }
    
    for (i = 0; i < num_strings; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        
        len = strlen(strings[i]);
        string_copy = malloc(len + 1);
        if (string_copy == NULL) {
            free(word_counts);
            return NULL;
        }
        memcpy(string_copy, strings[i], len + 1);
        
        token = strtok_r(string_copy, " \t\n\r", &saveptr);
        while (token != NULL) {
            int found = 0;
            
            for (j = 0; j < unique_words; j++) {
                if (strcmp(word_counts[j].word, token) == 0) {
                    word_counts[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && unique_words < MAX_UNIQUE_WORDS) {
                strncpy(word_counts[unique_words].word, token, MAX_WORD_LENGTH - 1);
                word_counts[unique_words].word[MAX_WORD_LENGTH - 1] = '\0';
                word_counts[unique_words].count = 1;
                unique_words++;
            }
            
            token = strtok_r(NULL, " \t\n\r", &saveptr);
        }
        
        free(string_copy);
    }
    
    if (unique_words == 0) {
        free(word_counts);
        return NULL;
    }
    
    max_count = word_counts[0].count;
    max_index = 0;
    
    for (k = 1; k < unique_words; k++) {
        if (word_counts[k].count > max_count) {
            max_count = word_counts[k].count;
            max_index = k;
        }
    }
    
    len = strlen(word_counts[max_index].word);
    result = malloc(len + 1);
    if (result == NULL) {
        free(word_counts);
        return NULL;
    }
    memcpy(result, word_counts[max_index].word, len + 1);
    
    free(word_counts);
    return result;
}

int main(void) {
    const char *strings[] = {
        "hello world hello",
        "world is beautiful world",
        "hello beautiful world"
    };
    int num_strings = sizeof(strings) / sizeof(strings[0]);
    char *most_frequent;
    
    most_frequent = get_most_frequent_word(strings, num_strings);
    
    if (most_frequent != NULL) {
        printf("Most frequent word: %s\n", most_frequent);
        free(most_frequent);
    } else {
        printf("No words found or error occurred\n");
    }
    
    return 0;
}