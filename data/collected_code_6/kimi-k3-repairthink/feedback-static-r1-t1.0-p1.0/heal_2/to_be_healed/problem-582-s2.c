#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    char *key;
    char *value;
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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t keyLen = strlen(key) + 1;
    newNode->key = (char *)malloc(keyLen);
    if (newNode->key == NULL) {
        free(newNode);
        return false;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(newNode->key, key, keyLen);

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t valueLen = strlen(value) + 1;
    newNode->value = (char *)malloc(valueLen);
    if (newNode->value == NULL) {
        free(newNode->key);
        free(newNode);
        return false;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(newNode->value, value, valueLen);

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
        free(current->key);
        free(current->value);
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