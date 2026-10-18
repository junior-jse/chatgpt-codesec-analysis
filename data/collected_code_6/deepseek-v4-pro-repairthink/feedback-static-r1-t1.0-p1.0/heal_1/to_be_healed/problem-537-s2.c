#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define HASH_SIZE 101

typedef struct WordNode {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    struct WordNode *next;
} WordNode;

unsigned int hash_word(const char *word) {
    unsigned int hash = 5381;
    int c;
    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + tolower((unsigned char)c);
    }
    return hash % HASH_SIZE;
}

WordNode *create_node(const char *word) {
    WordNode *node = (WordNode *)malloc(sizeof(WordNode));
    if (node == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(node->word, word, MAX_WORD_LEN - 1);
    node->word[MAX_WORD_LEN - 1] = '\0';
    node->next = NULL;
    return node;
}

void free_table(WordNode *table[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode *current = table[i];
        while (current != NULL) {
            WordNode *temp = current;
            current = current->next;
            free(temp);
        }
        table[i] = NULL;
    }
}

char *find_first_repeated_word(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    WordNode *hash_table[HASH_SIZE] = {0};
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    int word_index = 0;
    int in_word = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        
        if (isalnum((unsigned char)c) || c == '\'') {
            if (!in_word) {
                in_word = 1;
                word_index = 0;
            }
            if (word_index < MAX_WORD_LEN - 1) {
                word[word_index++] = c;
            }
        } else {
            if (in_word) {
                word[word_index] = '\0';
                in_word = 0;
                
                if (word_index > 0) {
                    unsigned int hash = hash_word(word);
                    WordNode *current = hash_table[hash];
                    
                    while (current != NULL) {
                        if (strcasecmp(current->word, word) == 0) {
                            /* Possible weaknesses found:
                             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                             */
                            char *result = (char *)malloc(strlen(word) + 1);
                            if (result != NULL) {
                                /* Possible weaknesses found:
                                 * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
                                 */
                                strcpy(result, word);
                            }
                            free_table(hash_table);
                            return result;
                        }
                        current = current->next;
                    }
                    
                    WordNode *new_node = create_node(word);
                    if (new_node == NULL) {
                        free_table(hash_table);
                        return NULL;
                    }
                    new_node->next = hash_table[hash];
                    hash_table[hash] = new_node;
                }
            }
        }
    }
    
    if (in_word && word_index > 0) {
        word[word_index] = '\0';
        unsigned int hash = hash_word(word);
        WordNode *current = hash_table[hash];
        
        while (current != NULL) {
            if (strcasecmp(current->word, word) == 0) {
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                char *result = (char *)malloc(strlen(word) + 1);
                if (result != NULL) {
                    /* Possible weaknesses found:
                     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
                     */
                    strcpy(result, word);
                }
                free_table(hash_table);
                return result;
            }
            current = current->next;
        }
    }
    
    free_table(hash_table);
    return NULL;
}

int main() {
    const char *test_strings[] = {
        "This is a test. This is only a test.",
        "Hello world, hello again.",
        "One two three four five.",
        "The cat and the dog and the bird.",
        "No repetition here.",
        "",
        "word word word",
        "Case CASE case test",
        NULL
    };
    
    for (int i = 0; test_strings[i] != NULL; i++) {
        char *result = find_first_repeated_word(test_strings[i]);
        printf("String: \"%s\"\n", test_strings[i]);
        if (result != NULL) {
            printf("First repeated word: \"%s\"\n\n", result);
            free(result);
        } else {
            printf("No repeated word found.\n\n");
        }
    }
    
    return 0;
}