#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_KEY_SIZE 100
#define MAX_VALUE_SIZE 100

typedef struct Node {
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} Dictionary;

void initDictionary(Dictionary *dict) {
    if (dict != NULL) {
        dict->head = NULL;
        dict->size = 0;
    }
}

bool isEmpty(const Dictionary *dict) {
    if (dict == NULL) {
        return true;
    }
    return (dict->head == NULL && dict->size == 0);
}

bool insert(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return false;
    }
    
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        return false;
    }
    
    strncpy(newNode->key, key, MAX_KEY_SIZE - 1);
    newNode->key[MAX_KEY_SIZE - 1] = '\0';
    strncpy(newNode->value, value, MAX_VALUE_SIZE - 1);
    newNode->value[MAX_VALUE_SIZE - 1] = '\0';
    
    newNode->next = dict->head;
    dict->head = newNode;
    dict->size++;
    
    return true;
}

void freeDictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    Node *current = dict->head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    dict->head = NULL;
    dict->size = 0;
}

int main(void) {
    Dictionary dict;
    initDictionary(&dict);
    
    if (isEmpty(&dict)) {
        printf("Dictionary is empty\n");
    } else {
        printf("Dictionary is not empty\n");
    }
    
    if (!insert(&dict, "name", "John")) {
        fprintf(stderr, "Failed to insert element\n");
        freeDictionary(&dict);
        return EXIT_FAILURE;
    }
    
    if (isEmpty(&dict)) {
        printf("Dictionary is empty\n");
    } else {
        printf("Dictionary is not empty\n");
    }
    
    freeDictionary(&dict);
    
    return EXIT_SUCCESS;
}