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

void remove_duplicate_words(const char *input, char *output) {
    WordList seen;
    init_word_list(&seen);

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    char *copy = malloc(input_len + 1);
    if (copy == NULL) {
        output[0] = '\0';
        return;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, input, input_len + 1);

    char *token = strtok(copy, " \t\n\r");
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[MAX_WORDS * MAX_WORD_LEN];
    int offset = 0;
    int first = 1;

    while (token != NULL) {
        if (!contains_word(&seen, token)) {
            add_word(&seen, token);
            int written;
            if (!first) {
                written = snprintf(result + offset, sizeof(result) - offset, " %s", token);
            } else {
                written = snprintf(result + offset, sizeof(result) - offset, "%s", token);
                first = 0;
            }
            if (written < 0 || (size_t)written >= sizeof(result) - offset) {
                break;
            }
            offset += written;
        }
        token = strtok(NULL, " \t\n\r");
    }

    snprintf(output, sizeof(output), "%s", result);
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

    remove_duplicate_words(input, output);

    printf("Original: %s\n", input);
    printf("After removing duplicates: %s\n", output);

    return 0;
}