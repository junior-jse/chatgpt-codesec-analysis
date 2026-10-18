#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    size_t count;
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
    
    if (list->count >= MAX_WORDS) {
        return -1;
    }
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return -1;
    }
    
    memcpy(list->words[list->count], word, len + 1);
    list->count++;
    return 0;
}

char *find_first_repeated_word(const char *str) {
    static char result[MAX_WORD_LEN];
    WordList word_list = {0};
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
                char lower_word[MAX_WORD_LEN];
                current_word[word_len] = '\0';
                to_lowercase(lower_word, sizeof(lower_word), current_word, word_len);
                
                if (find_word(&word_list, lower_word)) {
                    size_t copy_len = (word_len < sizeof(result) - 1) ? word_len : sizeof(result) - 1;
                    memcpy(result, current_word, copy_len);
                    result[copy_len] = '\0';
                    return result;
                }
                
                if (add_word(&word_list, lower_word) != 0) {
                    return NULL;
                }
                
                word_len = 0;
                in_word = false;
            }
        }
    }
    
    if (in_word && word_len > 0) {
        char lower_word[MAX_WORD_LEN];
        current_word[word_len] = '\0';
        to_lowercase(lower_word, sizeof(lower_word), current_word, word_len);
        
        if (find_word(&word_list, lower_word)) {
            size_t copy_len = (word_len < sizeof(result) - 1) ? word_len : sizeof(result) - 1;
            memcpy(result, current_word, copy_len);
            result[copy_len] = '\0';
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