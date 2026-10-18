#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 100
#define MAX_INPUT_LEN 262144

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
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
    if (list->count >= MAX_WORDS) return 0;
    
    size_t word_len = strnlen(word, MAX_WORD_LEN);
    if (word_len >= MAX_WORD_LEN) return 0;
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(list->words[list->count], word, word_len);
    list->words[list->count][word_len] = '\0';
    list->count++;
    return 1;
}

void remove_duplicate_words(const char *input, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return;
    }

    WordList seen;
    init_word_list(&seen);
    
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN) {
        output[0] = '\0';
        return;
    }
    
    char *copy = malloc(input_len + 1);
    if (copy == NULL) {
        output[0] = '\0';
        return;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, input, input_len);
    copy[input_len] = '\0';
    
    char *saveptr;
    char *token = strtok_r(copy, " \t\n\r", &saveptr);
    output[0] = '\0';
    int first = 1;
    size_t current_len = 0;
    
    while (token != NULL) {
        size_t token_len = strnlen(token, MAX_WORD_LEN - 1);
        
        if (!contains_word(&seen, token)) {
            add_word(&seen, token);
            
            size_t space_len = first ? 0 : 1;
            
            if (current_len + space_len + token_len >= output_size) {
                if (output_size > 0) {
                    output[current_len] = '\0';
                }
                free(copy);
                return;
            }
            
            if (!first) {
                output[current_len] = ' ';
                current_len++;
            }
            
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + current_len, token, token_len);
            current_len += token_len;
            output[current_len] = '\0';
            first = 0;
        }
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    
    if (output_size > 0) {
        output[current_len] = '\0';
    }
    free(copy);
}

int main(void) {
    char *input = NULL;
    size_t input_size = 0;
    char *output = NULL;
    
    printf("Enter a string: ");
    
    ssize_t read_len = getline(&input, &input_size, stdin);
    if (read_len == -1) {
        fprintf(stderr, "Error reading input.\n");
        free(input);
        return 1;
    }
    
    if (read_len > 0 && input[read_len - 1] == '\n') {
        input[read_len - 1] = '\0';
        read_len--;
    }
    
    size_t output_size = read_len + 1;
    if (output_size < 256) {
        output_size = 256;
    }
    
    output = malloc(output_size);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(input);
        return 1;
    }
    
    remove_duplicate_words(input, output, output_size);
    
    printf("Original: %s\n", input);
    printf("After removing duplicates: %s\n", output);
    
    free(input);
    free(output);
    
    return 0;
}