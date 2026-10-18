/* Repair history (previous round):
 * - Flawfinder strcpy was replaced by strncpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WordCount {
    char *word;
    int count;
    struct WordCount *next;
} WordCount;

static WordCount *find_word(WordCount *head, const char *word) {
    WordCount *current = head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static WordCount *add_word(WordCount *head, const char *word) {
    WordCount *new_node = (WordCount *)malloc(sizeof(WordCount));
    if (new_node == NULL) {
        return NULL;
    }

    size_t word_len = strnlen(word, 1024);
    new_node->word = (char *)malloc(word_len + 1);
    if (new_node->word == NULL) {
        free(new_node);
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(new_node->word, word, word_len);
    new_node->word[word_len] = '\0';
    new_node->count = 1;
    new_node->next = head;
    return new_node;
}

static void free_list(WordCount *head) {
    WordCount *current = head;
    while (current != NULL) {
        WordCount *next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}

char *most_frequent_word(const char *strings[], int size) {
    if (strings == NULL || size <= 0) {
        return NULL;
    }

    WordCount *head = NULL;
    for (int i = 0; i < size; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        WordCount *found = find_word(head, strings[i]);
        if (found != NULL) {
            found->count++;
        } else {
            WordCount *new_head = add_word(head, strings[i]);
            if (new_head == NULL) {
                free_list(head);
                return NULL;
            }
            head = new_head;
        }
    }

    if (head == NULL) {
        return NULL;
    }

    WordCount *current = head;
    WordCount *max_node = head;
    while (current != NULL) {
        if (current->count > max_node->count) {
            max_node = current;
        }
        current = current->next;
    }

    size_t result_len = strnlen(max_node->word, 1024);
    char *result = (char *)malloc(result_len + 1);
    if (result == NULL) {
        free_list(head);
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(result, max_node->word, result_len);
    result[result_len] = '\0';

    free_list(head);
    return result;
}

int main(void) {
    const char *strings[] = {"apple", "banana", "apple", "orange", "banana", "apple", "grape"};
    int size = sizeof(strings) / sizeof(strings[0]);
    
    char *result = most_frequent_word(strings, size);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    return 0;
}