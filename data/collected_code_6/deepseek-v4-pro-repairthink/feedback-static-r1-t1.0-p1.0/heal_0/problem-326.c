#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WordCount {
    char *word;
    int count;
    struct WordCount *next;
} WordCount;

static char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

static WordCount *find_word(WordCount *head, const char *word) {
    WordCount *current = head;
    while (current) {
        if (strcmp(current->word, word) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void free_list(WordCount *head) {
    while (head) {
        WordCount *next = head->next;
        free(head->word);
        free(head);
        head = next;
    }
}

char *most_frequent_word(char **strings, int count) {
    if (!strings || count <= 0) {
        return NULL;
    }

    WordCount *head = NULL;
    WordCount *tail = NULL;
    char *result = NULL;
    int i;

    for (i = 0; i < count; i++) {
        if (!strings[i]) {
            continue;
        }

        WordCount *found = find_word(head, strings[i]);
        if (found) {
            found->count++;
        } else {
            WordCount *new_node = malloc(sizeof(*new_node));
            if (!new_node) {
                free_list(head);
                return NULL;
            }
            new_node->word = my_strdup(strings[i]);
            if (!new_node->word) {
                free(new_node);
                free_list(head);
                return NULL;
            }
            new_node->count = 1;
            new_node->next = NULL;

            if (!head) {
                head = new_node;
            } else {
                tail->next = new_node;
            }
            tail = new_node;
        }
    }

    if (head) {
        WordCount *current = head;
        WordCount *max_node = head;
        while (current) {
            if (current->count > max_node->count) {
                max_node = current;
            }
            current = current->next;
        }
        result = my_strdup(max_node->word);
    }

    free_list(head);
    return result;
}

int main(void) {
    char *words[] = {"apple", "banana", "apple", "orange", "banana", "apple", "grape"};
    int count = sizeof(words) / sizeof(words[0]);
    char *result = most_frequent_word(words, count);

    if (result) {
        printf("%s\n", result);
        free(result);
    } else {
        printf("No word found\n");
    }

    return 0;
}