#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DictItem {
    char *key;
    char *value;
    struct DictItem *next;
} DictItem;

typedef struct {
    DictItem *head;
} Dictionary;

void dict_init(Dictionary *dict) {
    dict->head = NULL;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    
    DictItem *item = (DictItem *)malloc(sizeof(DictItem));
    if (item == NULL) {
        return -1;
    }
    
    item->key = (char *)malloc(strlen(key) + 1);
    if (item->key == NULL) {
        free(item);
        return -1;
    }
    
    item->value = (char *)malloc(strlen(value) + 1);
    if (item->value == NULL) {
        free(item->key);
        free(item);
        return -1;
    }
    
    strcpy(item->key, key);
    strcpy(item->value, value);
    item->next = dict->head;
    dict->head = item;
    
    return 0;
}

void dict_drop_empty(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    DictItem **current = &(dict->head);
    
    while (*current != NULL) {
        if ((*current)->value == NULL || strlen((*current)->value) == 0) {
            DictItem *to_delete = *current;
            *current = (*current)->next;
            free(to_delete->key);
            free(to_delete->value);
            free(to_delete);
        } else {
            current = &((*current)->next);
        }
    }
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    const DictItem *current = dict->head;
    while (current != NULL) {
        printf("%s: %s\n", current->key, current->value);
        current = current->next;
    }
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    DictItem *current = dict->head;
    while (current != NULL) {
        DictItem *next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    
    dict->head = NULL;
}

int main(void) {
    Dictionary dict;
    dict_init(&dict);
    
    dict_add(&dict, "name", "John");
    dict_add(&dict, "email", "");
    dict_add(&dict, "phone", "123-456-7890");
    dict_add(&dict, "address", "");
    dict_add(&dict, "city", "New York");
    
    printf("Before dropping empty items:\n");
    dict_print(&dict);
    
    dict_drop_empty(&dict);
    
    printf("\nAfter dropping empty items:\n");
    dict_print(&dict);
    
    dict_free(&dict);
    
    return 0;
}