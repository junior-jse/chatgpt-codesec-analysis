#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyValue {
    char *key;
    int value;
    struct KeyValue *next;
} KeyValue;

typedef struct {
    KeyValue *head;
    int size;
} Dictionary;

Dictionary *create_dictionary() {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->head = NULL;
    dict->size = 0;
    return dict;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    
    KeyValue *current = dict->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return 0;
        }
        current = current->next;
    }
    
    KeyValue *new_node = (KeyValue *)malloc(sizeof(KeyValue));
    if (new_node == NULL) {
        return -1;
    }
    
    new_node->key = (char *)malloc(strlen(key) + 1);
    if (new_node->key == NULL) {
        free(new_node);
        return -1;
    }
    
    strcpy(new_node->key, key);
    new_node->value = value;
    new_node->next = dict->head;
    dict->head = new_node;
    dict->size++;
    
    return 0;
}

char **dict_get_keys(Dictionary *dict, int *count) {
    if (dict == NULL || count == NULL) {
        return NULL;
    }
    
    *count = dict->size;
    if (dict->size == 0) {
        return NULL;
    }
    
    char **keys = (char **)malloc(sizeof(char *) * dict->size);
    if (keys == NULL) {
        return NULL;
    }
    
    KeyValue *current = dict->head;
    int index = 0;
    
    while (current != NULL && index < dict->size) {
        keys[index] = (char *)malloc(strlen(current->key) + 1);
        if (keys[index] == NULL) {
            for (int i = 0; i < index; i++) {
                free(keys[i]);
            }
            free(keys);
            return NULL;
        }
        strcpy(keys[index], current->key);
        index++;
        current = current->next;
    }
    
    return keys;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    KeyValue *current = dict->head;
    while (current != NULL) {
        KeyValue *temp = current;
        current = current->next;
        free(temp->key);
        free(temp);
    }
    
    free(dict);
}

void free_keys(char **keys, int count) {
    if (keys == NULL) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        free(keys[i]);
    }
    free(keys);
}

int main() {
    Dictionary *dict = create_dictionary();
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }
    
    dict_add(dict, "name", 1);
    dict_add(dict, "age", 2);
    dict_add(dict, "city", 3);
    dict_add(dict, "email", 4);
    
    int count = 0;
    char **keys = dict_get_keys(dict, &count);
    
    if (keys == NULL && count > 0) {
        fprintf(stderr, "Failed to get keys\n");
        dict_free(dict);
        return 1;
    }
    
    printf("Dictionary keys (%d):\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", i, keys[i]);
    }
    
    free_keys(keys, count);
    dict_free(dict);
    
    return 0;
}