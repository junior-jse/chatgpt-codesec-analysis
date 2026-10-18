#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *word;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} LinkedList;

void linked_list_init(LinkedList *list) {
    list->head = NULL;
}

int linked_list_contains(LinkedList *list, const char *word) {
    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int linked_list_insert(LinkedList *list, const char *word) {
    if (linked_list_contains(list, word)) {
        return 0;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->word = (char *)malloc(strlen(word) + 1);
    if (new_node->word == NULL) {
        free(new_node);
        return -1;
    }
    strcpy(new_node->word, word);
    new_node->next = list->head;
    list->head = new_node;
    return 0;
}

void linked_list_free(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
    list->head = NULL;
}

char *remove_duplicate_words(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    size_t len = strlen(str);
    char *temp = (char *)malloc(len + 1);
    if (temp == NULL) {
        return NULL;
    }
    
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        free(temp);
        return NULL;
    }
    
    result[0] = '\0';
    
    LinkedList collection;
    linked_list_init(&collection);
    
    strcpy(temp, str);
    
    char *token = strtok(temp, " ");
    while (token != NULL) {
        if (!linked_list_contains(&collection, token)) {
            if (linked_list_insert(&collection, token) != 0) {
                linked_list_free(&collection);
                free(temp);
                free(result);
                return NULL;
            }
            if (strlen(result) > 0) {
                strcat(result, " ");
            }
            strcat(result, token);
        }
        token = strtok(NULL, " ");
    }
    
    linked_list_free(&collection);
    free(temp);
    
    return result;
}

int main(int argc, char *argv[]) {
    const char *input = "hello world hello C world C programming";
    char *output = remove_duplicate_words(input);
    
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Input: %s\n", input);
    printf("Output: %s\n", output);
    
    free(output);
    
    return 0;
}