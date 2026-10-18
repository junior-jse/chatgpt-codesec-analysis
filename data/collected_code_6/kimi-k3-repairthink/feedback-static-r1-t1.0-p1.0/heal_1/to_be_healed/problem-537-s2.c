#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

void to_lowercase(char *dest, const char *src, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) {
        dest[i] = tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

int find_word(const WordList *list, const char *word) {
    int i;
    for (i = 0; i < list->count; i++) {
        if (strcmp(list->words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_word(WordList *list, const char *word) {
    size_t len;
    if (list->count >= MAX_WORDS) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(list->words[list->count], word, MAX_WORD_LEN - 1);
    list->words[list->count][MAX_WORD_LEN - 1] = '\0';
    list->count++;
    return 0;
}

char* find_first_repeated_word(const char *str) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    static char result[MAX_WORD_LEN];
    WordList word_list;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN];
    size_t i = 0;
    size_t word_len = 0;
    int in_word = 0;
    
    if (str == NULL) {
        return NULL;
    }
    
    word_list.count = 0;
    result[0] = '\0';
    
    while (str[i] != '\0') {
        if (is_word_char(str[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = str[i];
            }
            in_word = 1;
        } else {
            if (in_word && word_len > 0) {
                /* Possible weaknesses found:
                 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
                 */
                char lower_word[MAX_WORD_LEN];
                current_word[word_len] = '\0';
                to_lowercase(lower_word, current_word, word_len);
                
                if (find_word(&word_list, lower_word)) {
                    /* Possible weaknesses found:
                     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                     */
                    strncpy(result, current_word, MAX_WORD_LEN - 1);
                    result[MAX_WORD_LEN - 1] = '\0';
                    return result;
                }
                
                if (add_word(&word_list, lower_word) != 0) {
                    return NULL;
                }
                
                word_len = 0;
                in_word = 0;
            }
        }
        i++;
    }
    
    if (in_word && word_len > 0) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char lower_word[MAX_WORD_LEN];
        current_word[word_len] = '\0';
        to_lowercase(lower_word, current_word, word_len);
        
        if (find_word(&word_list, lower_word)) {
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result, current_word, MAX_WORD_LEN - 1);
            result[MAX_WORD_LEN - 1] = '\0';
            return result;
        }
    }
    
    return NULL;
}

int main(void) {
    const char *test_strings[] = {
        "The quick brown fox jumps over the lazy dog",
        "Hello world hello everyone",
        "one two three four five",
        "apple banana apple cherry",
        "This is a test this is only a test"
    };
    
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    size_t i;
    /* Possible weaknesses found:
     *  The scope of the variable 'repeated' can be reduced. [variableScope]
     */
    char *repeated;
    
    for (i = 0; i < num_tests; i++) {
        printf("String: \"%s\"\n", test_strings[i]);
        repeated = find_first_repeated_word(test_strings[i]);
        
        if (repeated != NULL && repeated[0] != '\0') {
            printf("First repeated word: \"%s\"\n\n", repeated);
        } else {
            printf("No repeated words found.\n\n");
        }
    }
    
    return 0;
}