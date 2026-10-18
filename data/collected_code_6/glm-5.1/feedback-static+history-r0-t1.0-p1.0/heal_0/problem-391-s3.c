#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dict {
    char *key;
    struct Dict *child;
    struct Dict *next;
} Dict;

Dict *create_node(const char *key) {
    Dict *node = malloc(sizeof(Dict));
    if (!node) return NULL;
    node->key = malloc(strlen(key) + 1);
    if (!node->key) {
        free(node);
        return NULL;
    }
    strcpy(node->key, key);
    node->child = NULL;
    node->next = NULL;
    return node;
}

Dict *find_or_add_child(Dict *parent, const char *key) {
    if (!parent) return NULL;
    Dict *current = parent->child;
    while (current) {
        if (strcmp(current->key, key) == 0) return current;
        current = current->next;
    }
    Dict *new_node = create_node(key);
    if (!new_node) return NULL;
    new_node->next = parent->child;
    parent->child = new_node;
    return new_node;
}

int lists_to_nested_dict(Dict **root, char **lists, int *sizes, int list_count) {
    if (!root || !lists || !sizes || list_count <= 0) return -1;
    
    if (!*root && list_count > 0 && sizes[0] > 0) {
        *root = create_node(lists[0][0]);
        if (!*root) return -1;
    }

    for (int i = 1; i < list_count; i++) {
        Dict *current = *root;
        for (int j = 0; j < sizes[i]; j++) {
            current = find_or_add_child(current, lists[i][j]);
            if (!current) return -1;
        }
    }
    return 0;
}

void free_dict(Dict *node) {
    if (!node) return;
    free_dict(node->child);
    free_dict(node->next);
    free(node->key);
    free(node);
}

void print_dict(Dict *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", node->key);
    print_dict(node->child, depth + 1);
    print_dict(node->next, depth);
}

int main() {
    char *list1[] = {"root"};
    char *list2[] = {"root", "A", "B"};
    char *list3[] = {"root", "A", "C"};
    char *list4[] = {"root", "D"};
    
    char **lists[] = {list1, list2, list3, list4};
    int sizes[] = {1, 3, 3, 2};
    int list_count = 4;

    Dict *root = NULL;
    int result = lists_to_nested_dict(&root, lists, sizes, list_count);
    
    if (result == 0) {
        print_dict(root, 0);
    }

    free_dict(root);
    return 0;
}