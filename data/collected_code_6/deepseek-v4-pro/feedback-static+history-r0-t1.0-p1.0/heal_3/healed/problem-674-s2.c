#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

void init_word_list(WordList *list) {
    if (list == NULL) return;
    list->count = 0;
    memset(list->words, 0, sizeof(list->words));
}

int contains_word(WordList *list, const char *word) {
    if (list == NULL || word == NULL) return 0;
    for (int i = 0; i < list->count; i++) {
        if (strncmp(list->words[i], word, MAX_WORD_LEN - 1) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_word(WordList *list, const char *word) {
    if (list == NULL || word == NULL) return 0;
    if (list->count < MAX_WORDS) {
        int written = snprintf(list->words[list->count], MAX_WORD_LEN, "%s", word);
        if (written < 0 || written >= MAX_WORD_LEN) {
            list->words[list->count][0] = '\0';
            return 0;
        }
        list->words[list->count][MAX_WORD_LEN - 1] = '\0';
        list->count++;
        return 1;
    }
    return 0;
}

void remove_duplicate_words(const char *input, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return;
    }

    WordList seen;
    init_word_list(&seen);
    
    size_t input_len = strnlen(input, 262144);
    char *copy = malloc(input_len + 1);
    if (copy == NULL) {
        output[0] = '\0';
        return;
    }
    copy[0] = '\0';
    strncat(copy, input, input_len);
    
    char *saveptr;
    char *token = strtok_r(copy, " \t\n\r", &saveptr);
    output[0] = '\0';
    int first = 1;
    size_t current_len = 0;
    
    while (token != NULL) {
        if (!contains_word(&seen, token)) {
            add_word(&seen, token);
            
            size_t token_len = strnlen(token, MAX_WORD_LEN);
            size_t space_len = first ? 0 : 1;
            
            if (current_len + space_len + token_len >= output_size) {
                free(copy);
                output[output_size - 1] = '\0';
                return;
            }
            
            if (!first) {
                size_t remaining = output_size - current_len;
                if (remaining > 1) {
                    strncat(output + current_len, " ", remaining - 1);
                    current_len++;
                }
            }
            
            size_t remaining = output_size - current_len;
            if (remaining > 1) {
                strncat(output + current_len, token, remaining - 1);
                current_len += token_len;
            }
            first = 0;
        }
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    
    output[current_len] = '\0';
    free(copy);
}

int main(void) {
    char input[2048];
    char output[2048];
    
    printf("Enter a string: ");
    if (fgets(input, (int)sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    remove_duplicate_words(input, output, sizeof(output));
    
    printf("Original: %s\n", input);
    printf("After removing duplicates: %s\n", output);
    
    return 0;
}