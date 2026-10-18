#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_UNIQUE_WORDS 1000

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

/* Possible weaknesses found:
 *  Parameter 'strings' can be declared as const array [constParameter]
 */
char *get_most_frequent_word(char *strings[], int num_strings) {
    WordCount word_counts[MAX_UNIQUE_WORDS];
    int unique_words = 0;
    int i, j, k;
    char *token;
    char *string_copy;
    char *saveptr;
    char *result;
    int max_count;
    int max_index;
    
    if (strings == NULL || num_strings <= 0) {
        return NULL;
    }
    
    memset(word_counts, 0, sizeof(word_counts));
    
    for (i = 0; i < num_strings; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        string_copy = malloc(strlen(strings[i]) + 1);
        if (string_copy == NULL) {
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(string_copy, strings[i]);
        
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
                /* Possible weaknesses found:
                 * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                 */
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
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    result = malloc(strlen(word_counts[max_index].word) + 1);
    if (result == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(result, word_counts[max_index].word);
    
    return result;
}

int main(void) {
    char *strings[] = {
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