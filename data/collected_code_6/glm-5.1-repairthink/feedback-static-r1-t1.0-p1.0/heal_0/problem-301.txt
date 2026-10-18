#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DictNode {
    char *key;
    struct Dict *value;
    struct DictNode *next;
} DictNode;

typedef struct Dict {
    DictNode *head;
} Dict;

Dict *dict_create() {
    Dict *d = (Dict *)malloc(sizeof(Dict));
    if (d == NULL) {
        exit(EXIT_FAILURE);
    }
    d->head = NULL;
    return d;
}

void dict_insert(Dict *d, const char *key, Dict *value) {
    if (d == NULL || key == NULL) {
        return;
    }
    DictNode *node = (DictNode *)malloc(sizeof(DictNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->key = strdup(key);
    if (node->key == NULL) {
        free(node);
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = d->head;
    d->head = node;
}

int find_depth(Dict *d) {
    if (d == NULL) {
        return 0;
    }
    int max_depth = 0;
    DictNode *current = d->head;
    while (current != NULL) {
        if (current->value != NULL) {
            int current_depth = find_depth(current->value);
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
        }
        current = current->next;
    }
    return 1 + max_depth;
}

void dict_free(Dict *d) {
    if (d == NULL) {
        return;
    }
    DictNode *current = d->head;
    while (current != NULL) {
        DictNode *next = current->next;
        free(current->key);
        dict_free(current->value);
        free(current);
        current = next;
    }
}

int main() {
    Dict *d1 = dict_create();
    Dict *d2 = dict_create();
    Dict *d3 = dict_create();
    Dict *d4 = dict_create();
    
    dict_insert(d3, "key3", NULL);
    dict_insert(d2, "key2", d3);
    dict_insert(d1, "key1", d2);
    dict_insert(d4, "keyA", d1);
    dict_insert(d4, "keyB", d3);
    
    int depth = find_depth(d4);
    printf("%d\n", depth);
    
    dict_free(d4);
    
    return 0;
}