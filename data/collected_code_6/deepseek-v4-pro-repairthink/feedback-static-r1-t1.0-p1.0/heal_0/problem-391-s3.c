#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    void *value;
    int is_dict;
    struct Node *next;
} Node;

typedef struct Dict {
    Node *head;
} Dict;

Node *create_node(const char *key, void *value, int is_dict) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->key = strdup(key);
    node->value = value;
    node->is_dict = is_dict;
    node->next = NULL;
    return node;
}

void dict_insert(Dict *dict, const char *key, void *value, int is_dict) {
    Node *node = create_node(key, value, is_dict);
    if (!node) return;
    node->next = dict->head;
    dict->head = node;
}

Node *dict_find(Dict *dict, const char *key) {
    Node *cur = dict->head;
    while (cur) {
        if (strcmp(cur->key, key) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

Dict *create_dict(void) {
    Dict *d = (Dict *)malloc(sizeof(Dict));
    if (!d) return NULL;
    d->head = NULL;
    return d;
}

void dict_free(Dict *dict) {
    if (!dict) return;
    Node *cur = dict->head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp->key);
        if (tmp->is_dict && tmp->value) {
            dict_free((Dict *)tmp->value);
        }
        free(tmp);
    }
    free(dict);
}

Dict *lists_to_nested_dict(char ***keys, int num_lists) {
    if (!keys || num_lists <= 0) return NULL;
    
    Dict *root = create_dict();
    if (!root) return NULL;
    
    for (int i = 0; i < num_lists; i++) {
        if (!keys[i] || !keys[i][0]) continue;
        
        Dict *current = root;
        int j = 0;
        while (keys[i][j] && keys[i][j + 1]) {
            Node *existing = dict_find(current, keys[i][j]);
            if (existing && existing->is_dict) {
                current = (Dict *)existing->value;
            } else {
                Dict *new_dict = create_dict();
                if (!new_dict) return root;
                dict_insert(current, keys[i][j], new_dict, 1);
                current = new_dict;
            }
            j++;
        }
        
        if (keys[i][j]) {
            if (!dict_find(current, keys[i][j])) {
                dict_insert(current, keys[i][j], NULL, 0);
            }
        }
    }
    
    return root;
}

void print_nested_dict(Dict *dict, int depth) {
    if (!dict) return;
    Node *cur = dict->head;
    while (cur) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("%s", cur->key);
        if (cur->is_dict && cur->value) {
            printf(":\n");
            print_nested_dict((Dict *)cur->value, depth + 1);
        } else {
            printf("\n");
        }
        cur = cur->next;
    }
}

int main(void) {
    char *list1[] = {"a", "b", "c", NULL};
    char *list2[] = {"a", "b", "d", NULL};
    char *list3[] = {"x", "y", NULL};
    char *list4[] = {"a", "z", NULL};
    
    char **lists[] = {list1, list2, list3, list4};
    
    Dict *result = lists_to_nested_dict(lists, 4);
    if (result) {
        print_nested_dict(result, 0);
        dict_free(result);
    }
    
    return 0;
}