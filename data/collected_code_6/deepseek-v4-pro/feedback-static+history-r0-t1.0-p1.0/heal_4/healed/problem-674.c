#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

static int is_duplicate(const WordList *list, const char *word) {
    for (int i = 0; i < list->count; i++) {
        if (strncmp(list->words[i], word, MAX_WORD_LEN - 1) == 0) {
            return 1;
        }
    }
    return 0;
}

static int add_word(WordList *list, const char *word) {
    if (list->count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strnlen(word, MAX_WORD_LEN - 1);
    if (len >= MAX_WORD_LEN - 1) {
        return 0;
    }
    
    if (list->count >= MAX_WORDS) {
        return 0;
    }
    
    memcpy(list->words[list->count], word, len);
    list->words[list->count][len] = '\0';
    list->count++;
    return 1;
}

static size_t safe_strnlen(const char *s, size_t maxlen) {
    size_t i;
    for (i = 0; i < maxlen && s[i] != '\0'; i++);
    return i;
}

static void append_to_result(char *result, size_t result_capacity, size_t *result_len, const char *s, size_t s_len) {
    if (result == NULL || result_len == NULL || s == NULL) {
        return;
    }
    
    if (result_capacity <= *result_len) {
        return;
    }
    
    size_t avail = result_capacity - *result_len;
    if (avail <= 1) {
        return;
    }
    
    size_t to_copy = s_len < avail - 1 ? s_len : avail - 1;
    
    if (to_copy > 0) {
        memcpy(result + *result_len, s, to_copy);
        *result_len += to_copy;
        result[*result_len] = '\0';
    }
}

void remove_duplicate_words(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    WordList seen;
    memset(&seen, 0, sizeof(seen));
    
    size_t input_len = safe_strnlen(str, SIZE_MAX);
    if (input_len == 0) {
        return;
    }
    
    size_t result_capacity = input_len + 1;
    char *result = (char *)malloc(result_capacity);
    if (result == NULL) {
        return;
    }
    result[0] = '\0';

    size_t result_len = 0;

    char *saveptr;
    char *token = strtok_r(str, " \t\n\r", &saveptr);
    int first = 1;

    while (token != NULL) {
        if (!is_duplicate(&seen, token)) {
            add_word(&seen, token);
            
            if (!first) {
                append_to_result(result, result_capacity, &result_len, " ", 1);
            }
            
            size_t token_len = safe_strnlen(token, MAX_WORD_LEN - 1);
            append_to_result(result, result_capacity, &result_len, token, token_len);
            first = 0;
        }
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    size_t final_len = safe_strnlen(result, result_len);
    if (final_len <= input_len) {
        memmove(str, result, final_len);
        str[final_len] = '\0';
    }
    free(result);
}

int main(void) {
    char str[] = "hello world hello again world test hello";
    
    printf("Original: %s\n", str);
    remove_duplicate_words(str);
    printf("After removing duplicates: %s\n", str);

    return 0;
}