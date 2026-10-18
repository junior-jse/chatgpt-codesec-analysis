#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyValue {
    char *key;
    void *value;
    struct KeyValue *next;
} KeyValue;

typedef struct {
    KeyValue *head;
    int size;
} Dictionary;

Dictionary *dict_create(void) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->head = NULL;
    dict->size = 0;
    return dict;
}

int dict_insert(Dictionary *dict, const char *key, void *value) {
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

void *dict_get_by_index(Dictionary *dict, int index) {
    if (dict == NULL || index < 0 || index >= dict->size) {
        return NULL;
    }
    
    KeyValue *current = dict->head;
    int current_index = 0;
    
    while (current != NULL) {
        if (current_index == index) {
            return current->value;
        }
        current = current->next;
        current_index++;
    }
    
    return NULL;
}

void dict_destroy(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    KeyValue *current = dict->head;
    while (current != NULL) {
        KeyValue *next = current->next;
        free(current->key);
        free(current);
        current = next;
    }
    
    free(dict);
}

int main(void) {
    Dictionary *dict = dict_create();
    if (dict == NULL) {
        return 1;
    }
    
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    
    if (dict_insert(dict, "first", &value1) != 0 ||
        dict_insert(dict, "second", &value2) != 0 ||
        dict_insert(dict, "third", &value3) != 0) {
        dict_destroy(dict);
        return 1;
    }
    
    for (int i = 0; i < dict->size; i++) {
        int *value = (int *)dict_get_by_index(dict, i);
        if (value != NULL) {
            printf("Index %d: %d\n", i, *value);
        }
    }
    
    void *invalid = dict_get_by_index(dict, 5);
    if (invalid == NULL) {
        printf("Index 5: (null)\n");
    }
    
    dict_destroy(dict);
    return 0;
}