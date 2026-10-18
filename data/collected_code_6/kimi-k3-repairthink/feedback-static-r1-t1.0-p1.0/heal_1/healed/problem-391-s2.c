#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 1024
#define OUTER_KEY_SIZE 32

typedef struct Node {
    char *key;
    char *value;
    struct Node *child;
    struct Node *next;
} Node;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *safe_strdup(const char *s) {
    size_t len;
    char *copy;
    if (!s) {
        return NULL;
    }
    len = bounded_strlen(s, MAX_STRING_LEN);
    if (len >= MAX_STRING_LEN) {
        return NULL;
    }
    copy = malloc(len + 1);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

Node* create_node(const char *key, const char *value) {
    Node *node;
    if (!key || !value) {
        return NULL;
    }
    node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->key = safe_strdup(key);
    node->value = safe_strdup(value);
    if (!node->key || !node->value) {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }
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
    Node *cur;
    if (!key || bounded_strlen(key, MAX_STRING_LEN) >= MAX_STRING_LEN) {
        return NULL;
    }
    cur = parent ? parent->child : NULL;
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int add_to_dict(Node *parent, const char *key, const char *value) {
    Node *child;
    if (!parent || !key || !value) {
        return 0;
    }
    child = find_child(parent, key);
    if (!child) {
        child = create_node(key, value);
        if (!child) {
            return 0;
        }
        child->next = parent->child;
        parent->child = child;
    } else {
        char *new_value = safe_strdup(value);
        if (!new_value) {
            return 0;
        }
        free(child->value);
        child->value = new_value;
    }
    return 1;
}

Node* convert_lists_to_nested_dict(char **keys, char **values, int outer_size, int inner_size) {
    Node *root;
    if (!keys || !values || outer_size < 0 || inner_size < 0) {
        return NULL;
    }
    root = create_node("root", "");
    if (!root) {
        return NULL;
    }
    for (int i = 0; i < outer_size; i++) {
        char outer_key[OUTER_KEY_SIZE];
        int written = snprintf(outer_key, sizeof(outer_key), "item%d", i);
        Node *child;
        if (written < 0 || (size_t)written >= sizeof(outer_key)) {
            free_dict(root);
            return NULL;
        }
        child = find_child(root, outer_key);
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
            size_t idx = (size_t)i * (size_t)inner_size + (size_t)j;
            if (!keys[idx] || !values[idx]) {
                free_dict(root);
                return NULL;
            }
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