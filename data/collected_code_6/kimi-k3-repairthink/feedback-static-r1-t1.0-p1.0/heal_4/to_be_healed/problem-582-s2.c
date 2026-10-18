#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_LEN 1024

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

    size_t keyLen = strnlen(key, MAX_STRING_LEN);
    if (keyLen >= MAX_STRING_LEN) {
        return false;
    }
    keyLen += 1;

    size_t valueLen = strnlen(value, MAX_STRING_LEN);
    if (valueLen >= MAX_STRING_LEN) {
        return false;
    }
    valueLen += 1;

    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        return false;
    }

    newNode->key = (char *)malloc(keyLen);
    if (newNode->key == NULL) {
        free(newNode);
        return false;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(newNode->key, key, keyLen - 1);
    newNode->key[keyLen - 1] = '\0';

    newNode->value = (char *)malloc(valueLen);
    if (newNode->value == NULL) {
        free(newNode->key);
        free(newNode);
        return false;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(newNode->value, value, valueLen - 1);
    newNode->value[valueLen - 1] = '\0';

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