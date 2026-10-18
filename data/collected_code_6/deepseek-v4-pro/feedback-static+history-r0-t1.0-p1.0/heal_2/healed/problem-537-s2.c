#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define HASH_SIZE 101

typedef struct WordNode {
    char word[MAX_WORD_LEN];
    struct WordNode *next;
} WordNode;

static unsigned int hash_word(const char *word) {
    unsigned int hash = 5381;
    int c;
    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + tolower((unsigned char)c);
    }
    return hash % HASH_SIZE;
}

static WordNode *create_node(const char *word) {
    WordNode *node = (WordNode *)malloc(sizeof(WordNode));
    if (node == NULL) {
        return NULL;
    }
    size_t len = strnlen(word, MAX_WORD_LEN);
    if (len >= MAX_WORD_LEN) {
        len = MAX_WORD_LEN - 1;
    }
    memcpy(node->word, word, len);
    node->word[len] = '\0';
    node->next = NULL;
    return node;
}

static void free_table(WordNode *table[]) {
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

static char *duplicate_word(const char *word) {
    size_t len = strnlen(word, MAX_WORD_LEN);
    char *result = (char *)malloc(len + 1);
    if (result != NULL) {
        memcpy(result, word, len);
        result[len] = '\0';
    }
    return result;
}

char *find_first_repeated_word(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    WordNode *hash_table[HASH_SIZE] = {0};
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
                            char *result = duplicate_word(word);
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
                char *result = duplicate_word(word);
                free_table(hash_table);
                return result;
            }
            current = current->next;
        }
    }
    
    free_table(hash_table);
    return NULL;
}

int main(void) {
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