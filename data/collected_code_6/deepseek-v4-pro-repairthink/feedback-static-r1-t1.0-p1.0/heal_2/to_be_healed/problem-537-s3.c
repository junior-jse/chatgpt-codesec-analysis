#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1024
#define MAX_WORD_LEN 256

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    int count;
} WordEntry;

/* Possible weaknesses found:
 *  Parameter 'str' can be declared as pointer to const [constParameterPointer]
 */
char *first_repeated_word(char *str) {
    WordEntry *words;
    int word_count = 0;
    int i = 0;
    char *result = NULL;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN];
    int current_len = 0;
    
    if (str == NULL) {
        return NULL;
    }
    
    words = (WordEntry *)malloc(MAX_WORDS * sizeof(WordEntry));
    if (words == NULL) {
        return NULL;
    }
    
    while (str[i] != '\0') {
        if (isalpha((unsigned char)str[i])) {
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = tolower((unsigned char)str[i]);
            }
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
                            free(words);
                            return result;
                        }
                        break;
                    }
                }
                
                if (!found && word_count < MAX_WORDS) {
                    snprintf(words[word_count].word, MAX_WORD_LEN, "%s", current_word);
                    words[word_count].count = 1;
                    word_count++;
                }
                
                current_len = 0;
            }
        }
        i++;
    }
    
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
                    free(words);
                    return result;
                }
                break;
            }
        }
        
        if (!found && word_count < MAX_WORDS) {
            snprintf(words[word_count].word, MAX_WORD_LEN, "%s", current_word);
            words[word_count].count = 1;
            /* Possible weaknesses found:
             *  Variable 'word_count' is assigned a value that is never used. [unreadVariable]
             */
            word_count++;
        }
    }
    
    free(words);
    return NULL;
}

int main(void) {
    char str1[] = "This is a test. This is only a test.";
    char str2[] = "Hello world, hello everyone.";
    char str3[] = "No repeated words here.";
    char str4[] = "The cat and the dog and the bird";
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