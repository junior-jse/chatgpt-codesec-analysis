#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000

static const char *DELIMITERS = " \t\n\r.,;:!?\"'()[]{}";

char *find_first_repeated_word(const char *str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char *words[MAX_WORDS];
    char *str_copy = strdup(str);
    if (str_copy == NULL) {
        return NULL;
    }

    int word_count = 0;
    char *token = strtok(str_copy, DELIMITERS);

    while (token != NULL && word_count < MAX_WORDS) {
        char *lower_token = strdup(token);
        if (lower_token == NULL) {
            free(str_copy);
            for (int i = 0; i < word_count; i++) {
                free(words[i]);
            }
            return NULL;
        }

        for (int i = 0; lower_token[i]; i++) {
            lower_token[i] = tolower(lower_token[i]);
        }

        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], lower_token) == 0) {
                char *result = strdup(token);
                free(lower_token);
                free(str_copy);
                for (int j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                return result;
            }
        }

        words[word_count] = lower_token;
        word_count++;
        token = strtok(NULL, DELIMITERS);
    }

    free(str_copy);
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }

    return NULL;
}

static char *read_line(void) {
    size_t size = 128;
    size_t len = 0;
    char *buffer = malloc(size);
    if (!buffer) {
        return NULL;
    }

    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != '\n' && c != EOF) {
        if (len + 1 >= size) {
            size *= 2;
            char *temp = realloc(buffer, size);
            if (!temp) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[len++] = (char)c;
    }

    if (len == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';
    return buffer;
}

int main(void) {
    printf("Enter a string: ");
    char *input = read_line();
    if (input == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    char *result = find_first_repeated_word(input);
    if (result != NULL) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated word found.\n");
    }

    free(input);
    return 0;
}