#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define HASH_SIZE 101
#define MAX_STR_LEN 1048576

typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

static size_t bounded_strlen(const char *s, size_t maxlen) {
    size_t len = 0;
    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *string_dup(const char *s) {
    if (s == NULL) return NULL;
    size_t len = bounded_strlen(s, MAX_STR_LEN);
    char *dup = malloc(len + 1);
    if (dup != NULL) {
        memcpy(dup, s, len);
        dup[len] = '\0';
    }
    return dup;
}

unsigned int hash_word(const char *word) {
    unsigned int hash = 5381;
    int c;
    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + tolower((unsigned char)c);
    }
    return hash % HASH_SIZE;
}

WordNode *create_node(const char *word) {
    WordNode *node = malloc(sizeof(WordNode));
    if (node == NULL) return NULL;
    node->word = string_dup(word);
    if (node->word == NULL) {
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

void free_table(WordNode *table[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode *current = table[i];
        while (current != NULL) {
            WordNode *temp = current;
            current = current->next;
            free(temp->word);
            free(temp);
        }
        table[i] = NULL;
    }
}

char *find_first_repeated_word(const char *str) {
    if (str == NULL) return NULL;

    WordNode *hash_table[HASH_SIZE] = {0};
    size_t str_len = bounded_strlen(str, MAX_STR_LEN);
    char *word = malloc(str_len + 1);
    if (word == NULL) return NULL;

    size_t word_index = 0;
    int in_word = 0;

    for (size_t i = 0; i < str_len; i++) {
        char c = str[i];
        if (isalnum((unsigned char)c) || c == '\'') {
            if (!in_word) {
                in_word = 1;
                word_index = 0;
            }
            if (word_index < str_len) {
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
                            char *result = string_dup(word);
                            free_table(hash_table);
                            free(word);
                            return result;
                        }
                        current = current->next;
                    }
                    WordNode *new_node = create_node(word);
                    if (new_node == NULL) {
                        free_table(hash_table);
                        free(word);
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
                char *result = string_dup(word);
                free_table(hash_table);
                free(word);
                return result;
            }
            current = current->next;
        }
    }

    free_table(hash_table);
    free(word);
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