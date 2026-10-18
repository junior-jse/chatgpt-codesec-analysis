#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    char *value;
    struct Node *child;
    struct Node *next;
} Node;

Node* create_node(const char *key, const char *value) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    node->key = malloc(strlen(key) + 1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    node->value = malloc(strlen(value) + 1);
    if (!node->key || !node->value) {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(node->key, key);
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(node->value, value);
    node->child = NULL;
    node->next = NULL;
    return node;
}

void free_dict(Node *node) {
    while (node) {
        Node *tmp = node;
        node = node->next;
        free_dict(tmp->child);
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

Node* find_child(Node *parent, const char *key) {
    Node *cur = parent ? parent->child : NULL;
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int add_to_dict(Node *parent, const char *key, const char *value) {
    Node *child = find_child(parent, key);
    if (!child) {
        child = create_node(key, value);
        if (!child) {
            return 0;
        }
        child->next = parent->child;
        parent->child = child;
    } else {
        free(child->value);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        child->value = malloc(strlen(value) + 1);
        if (!child->value) {
            return 0;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(child->value, value);
    }
    return 1;
}

Node* convert_lists_to_nested_dict(char **keys, char **values, int outer_size, int inner_size) {
    Node *root = create_node("root", "");
    if (!root) {
        return NULL;
    }
    for (int i = 0; i < outer_size; i++) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char outer_key[32];
        snprintf(outer_key, sizeof(outer_key), "item%d", i);
        Node *child = find_child(root, outer_key);
        if (!child) {
            child = create_node(outer_key, "");
            if (!child) {
                free_dict(root);
                return NULL;
            }
            child->next = root->child;
            root->child = child;
        }
        for (int j = 0; j < inner_size; j++) {
            int idx = i * inner_size + j;
            if (!add_to_dict(child, keys[idx], values[idx])) {
                free_dict(root);
                return NULL;
            }
        }
    }
    return root;
}

void print_dict(Node *node, int depth) {
    while (node) {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("%s: %s\n", node->key, node->value);
        print_dict(node->child, depth + 1);
        node = node->next;
    }
}

int main(void) {
    char *keys[] = {"name", "age", "city", "name", "age", "city"};
    char *values[] = {"Alice", "30", "NY", "Bob", "25", "LA"};
    int outer_size = 2;
    int inner_size = 3;
    Node *dict = convert_lists_to_nested_dict(keys, values, outer_size, inner_size);
    if (!dict) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }
    print_dict(dict, 0);
    free_dict(dict);
    return EXIT_SUCCESS;
}