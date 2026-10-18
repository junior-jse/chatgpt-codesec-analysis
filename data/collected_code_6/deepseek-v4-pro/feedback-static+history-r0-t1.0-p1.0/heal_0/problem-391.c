#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DictNode {
    char *key;
    void *value;
    int is_dict;
    struct DictNode *next;
} DictNode;

typedef struct {
    DictNode *head;
} Dict;

Dict *dict_create(void) {
    Dict *d = (Dict *)malloc(sizeof(Dict));
    if (!d) return NULL;
    d->head = NULL;
    return d;
}

void dict_set(Dict *d, const char *key, void *value, int is_dict) {
    if (!d || !key) return;
    DictNode *node = d->head;
    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            node->is_dict = is_dict;
            return;
        }
        node = node->next;
    }
    DictNode *new_node = (DictNode *)malloc(sizeof(DictNode));
    if (!new_node) return;
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->is_dict = is_dict;
    new_node->next = d->head;
    d->head = new_node;
}

DictNode *dict_get_node(Dict *d, const char *key) {
    if (!d || !key) return NULL;
    DictNode *node = d->head;
    while (node) {
        if (strcmp(node->key, key) == 0) return node;
        node = node->next;
    }
    return NULL;
}

void dict_free(Dict *d) {
    if (!d) return;
    DictNode *node = d->head;
    while (node) {
        DictNode *tmp = node;
        node = node->next;
        free(tmp->key);
        if (tmp->is_dict) {
            dict_free((Dict *)tmp->value);
        } else {
            free(tmp->value);
        }
        free(tmp);
    }
    free(d);
}

Dict *lists_to_nested_dict(char **keys, int *levels, char **values, int count) {
    if (!keys || !levels || !values || count <= 0) return NULL;
    Dict *root = dict_create();
    if (!root) return NULL;

    for (int i = 0; i < count; i++) {
        Dict *current = root;
        int depth = levels[i];
        if (depth < 1) continue;
        for (int j = 0; j < depth; j++) {
            char *key_part = keys[i];
            DictNode *node = dict_get_node(current, key_part);
            if (j == depth - 1) {
                if (!node) {
                    char *val = strdup(values[i]);
                    if (!val) continue;
                    dict_set(current, key_part, val, 0);
                } else {
                    if (!node->is_dict) {
                        free(node->value);
                        char *val = strdup(values[i]);
                        if (!val) continue;
                        node->value = val;
                        node->is_dict = 0;
                    }
                }
            } else {
                if (!node) {
                    Dict *sub = dict_create();
                    if (!sub) continue;
                    dict_set(current, key_part, sub, 1);
                    current = sub;
                } else {
                    if (!node->is_dict) {
                        free(node->value);
                        Dict *sub = dict_create();
                        if (!sub) continue;
                        node->value = sub;
                        node->is_dict = 1;
                    }
                    current = (Dict *)node->value;
                }
            }
        }
    }
    return root;
}

void dict_print(Dict *d, int level) {
    if (!d) return;
    DictNode *node = d->head;
    while (node) {
        for (int i = 0; i < level; i++) printf("  ");
        printf("%s: ", node->key);
        if (node->is_dict) {
            printf("{\n");
            dict_print((Dict *)node->value, level + 1);
            for (int i = 0; i < level; i++) printf("  ");
            printf("}\n");
        } else {
            printf("%s\n", (char *)node->value);
        }
        node = node->next;
    }
}

int main(void) {
    char *keys[] = {"a", "b", "c"};
    int levels[] = {3, 2, 1};
    char *values[] = {"1", "2", "3"};
    Dict *d = lists_to_nested_dict(keys, levels, values, 3);
    if (d) {
        dict_print(d, 0);
        dict_free(d);
    }
    return 0;
}