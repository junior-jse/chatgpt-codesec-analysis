#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1024
#define MAX_WORD_LEN 256

typedef struct {
    char *word;
    int count;
} WordEntry;

char *first_repeated_word(const char *str) {
    WordEntry *words = NULL;
    int word_count = 0;
    size_t i = 0;
    size_t len;
    char *result = NULL;
    char *current_word = NULL;
    size_t current_len = 0;
    size_t current_capacity = 0;
    
    if (str == NULL) {
        return NULL;
    }
    
    len = strlen(str);
    current_capacity = 64;
    current_word = malloc(current_capacity);
    if (current_word == NULL) {
        return NULL;
    }
    
    words = malloc(MAX_WORDS * sizeof(WordEntry));
    if (words == NULL) {
        free(current_word);
        return NULL;
    }
    
    for (i = 0; i < MAX_WORDS; i++) {
        words[i].word = NULL;
        words[i].count = 0;
    }
    
    for (i = 0; i <= len; i++) {
        if (i < len && isalpha((unsigned char)str[i])) {
            if (current_len + 1 >= current_capacity) {
                char *tmp;
                current_capacity *= 2;
                tmp = realloc(current_word, current_capacity);
                if (tmp == NULL) {
                    int j;
                    for (j = 0; j < word_count; j++) {
                        free(words[j].word);
                    }
                    free(words);
                    free(current_word);
                    return NULL;
                }
                current_word = tmp;
            }
            current_word[current_len++] = (char)tolower((unsigned char)str[i]);
        } else {
            if (current_len > 0) {
                int j;
                int found = 0;
                current_word[current_len] = '\0';
                
                for (j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        if (words[j].count == 2) {
                            result = strdup(words[j].word);
                            for (j = 0; j < word_count; j++) {
                                free(words[j].word);
                            }
                            free(words);
                            free(current_word);
                            return result;
                        }
                        break;
                    }
                }
                
                if (!found && word_count < MAX_WORDS) {
                    words[word_count].word = strdup(current_word);
                    if (words[word_count].word == NULL) {
                        int k;
                        for (k = 0; k < word_count; k++) {
                            free(words[k].word);
                        }
                        free(words);
                        free(current_word);
                        return NULL;
                    }
                    words[word_count].count = 1;
                    word_count++;
                }
                
                current_len = 0;
            }
        }
    }
    
    {
        int j;
        for (j = 0; j < word_count; j++) {
            free(words[j].word);
        }
    }
    free(words);
    free(current_word);
    return NULL;
}

int main(void) {
    const char str1[] = "This is a test. This is only a test.";
    const char str2[] = "Hello world, hello everyone.";
    const char str3[] = "No repeated words here.";
    const char str4[] = "The cat and the dog and the bird";
    char *result;
    
    result = first_repeated_word(str1);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }
    
    result = first_repeated_word(str2);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }
    
    result = first_repeated_word(str3);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }
    
    result = first_repeated_word(str4);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }
    
    result = first_repeated_word(NULL);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }
    
    return 0;
}