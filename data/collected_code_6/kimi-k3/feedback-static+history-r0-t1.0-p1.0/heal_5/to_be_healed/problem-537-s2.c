/* Repair history (previous round):
 * - Flawfinder memcpy was replaced by strncpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} WordList;

static bool is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

static void to_lowercase(char *dest, size_t dest_size, const char *src, size_t len) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }
    
    size_t copy_len = (len < dest_size - 1) ? len : dest_size - 1;
    for (size_t i = 0; i < copy_len; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[copy_len] = '\0';
}

static bool find_word(const WordList *list, const char *word) {
    if (list == NULL || word == NULL) {
        return false;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

static int add_word(WordList *list, const char *word) {
    if (list == NULL || word == NULL) {
        return -1;
    }
    
    size_t len = strnlen(word, MAX_WORD_LEN);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return -1;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = (list->capacity == 0) ? 16 : list->capacity * 2;
        if (new_capacity > MAX_WORDS) {
            new_capacity = MAX_WORDS;
        }
        if (list->count >= new_capacity) {
            return -1;
        }
        char **new_words = realloc(list->words, new_capacity * sizeof(char *));
        if (new_words == NULL) {
            return -1;
        }
        list->words = new_words;
        list->capacity = new_capacity;
    }
    
    list->words[list->count] = malloc(len + 1);
    if (list->words[list->count] == NULL) {
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(list->words[list->count], word, len);
    list->words[list->count][len] = '\0';
    list->count++;
    return 0;
}

static void free_word_list(WordList *list) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        free(list->words[i]);
    }
    free(list->words);
    list->words = NULL;
    list->count = 0;
    list->capacity = 0;
}

char *find_first_repeated_word(const char *str) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    static char result[MAX_WORD_LEN];
    WordList word_list = {0};
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN] = {0};
    size_t word_len = 0;
    bool in_word = false;
    
    if (str == NULL) {
        return NULL;
    }
    
    result[0] = '\0';
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (is_word_char(str[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = str[i];
            }
            in_word = true;
        } else {
            if (in_word && word_len > 0) {
                /* Possible weaknesses found:
                 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
                 */
                char lower_word[MAX_WORD_LEN];
                current_word[word_len] = '\0';
                to_lowercase(lower_word, sizeof(lower_word), current_word, word_len);
                
                if (find_word(&word_list, lower_word)) {
                    size_t copy_len = (word_len < sizeof(result) - 1) ? word_len : sizeof(result) - 1;
                    /* Possible weaknesses found:
                     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                     */
                    strncpy(result, current_word, copy_len);
                    result[copy_len] = '\0';
                    free_word_list(&word_list);
                    return result;
                }
                
                if (add_word(&word_list, lower_word) != 0) {
                    free_word_list(&word_list);
                    return NULL;
                }
                
                word_len = 0;
                in_word = false;
            }
        }
    }
    
    if (in_word && word_len > 0) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char lower_word[MAX_WORD_LEN];
        current_word[word_len] = '\0';
        to_lowercase(lower_word, sizeof(lower_word), current_word, word_len);
        
        if (find_word(&word_list, lower_word)) {
            size_t copy_len = (word_len < sizeof(result) - 1) ? word_len : sizeof(result) - 1;
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result, current_word, copy_len);
            result[copy_len] = '\0';
            free_word_list(&word_list);
            return result;
        }
    }
    
    free_word_list(&word_list);
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
    
    for (size_t i = 0; i < num_tests; i++) {
        printf("String: \"%s\"\n", test_strings[i]);
        char *repeated = find_first_repeated_word(test_strings[i]);
        
        if (repeated != NULL && repeated[0] != '\0') {
            printf("First repeated word: \"%s\"\n\n", repeated);
        } else {
            printf("No repeated words found.\n\n");
        }
    }
    
    return 0;
}