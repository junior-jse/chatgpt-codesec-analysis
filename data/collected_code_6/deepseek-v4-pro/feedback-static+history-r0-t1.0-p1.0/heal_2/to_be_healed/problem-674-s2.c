/* Repair history (previous round):
 * - Flawfinder strcat was replaced by strncat, which Flawfinder flags with the same CWE (CWE-120).
 * - Flawfinder strcpy was replaced by strncat, which Flawfinder flags with the same CWE (CWE-120).
 * - Flawfinder strncpy was replaced by strncat, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

void init_word_list(WordList *list) {
    list->count = 0;
}

int contains_word(WordList *list, const char *word) {
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_word(WordList *list, const char *word) {
    if (list->count < MAX_WORDS) {
        snprintf(list->words[list->count], MAX_WORD_LEN, "%s", word);
        list->count++;
    }
}

void remove_duplicate_words(const char *input, char *output, size_t output_size) {
    WordList seen;
    init_word_list(&seen);
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    char *copy = malloc(strlen(input) + 1);
    if (copy == NULL) {
        output[0] = '\0';
        return;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    snprintf(copy, strlen(input) + 1, "%s", input);
    
    char *saveptr;
    char *token = strtok_r(copy, " \t\n\r", &saveptr);
    output[0] = '\0';
    int first = 1;
    size_t current_len = 0;
    
    while (token != NULL) {
        if (!contains_word(&seen, token)) {
            add_word(&seen, token);
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t token_len = strlen(token);
            size_t space_len = first ? 0 : 1;
            
            if (current_len + space_len + token_len >= output_size) {
                free(copy);
                return;
            }
            
            if (!first) {
                /* Possible weaknesses found:
                 * Flawfinder strncat: Easily used incorrectly (e.g., incorrectly computing the correct maximum size to add) [MS-banned] (CWE-120). Consider strcat_s, strlcat, snprintf, or automatically resizing strings. Risk is low because the source is a constant character. (risk 1, buffer)
                 */
                strncat(output, " ", output_size - current_len - 1);
                current_len++;
            }
            /* Possible weaknesses found:
             * Flawfinder strncat: Easily used incorrectly (e.g., incorrectly computing the correct maximum size to add) [MS-banned] (CWE-120). Consider strcat_s, strlcat, snprintf, or automatically resizing strings. (risk 1, buffer)
             */
            strncat(output, token, output_size - current_len - 1);
            current_len += token_len;
            first = 0;
        }
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    
    free(copy);
}

int main() {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[2048];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char output[2048];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    remove_duplicate_words(input, output, sizeof(output));
    
    printf("Original: %s\n", input);
    printf("After removing duplicates: %s\n", output);
    
    return 0;
}