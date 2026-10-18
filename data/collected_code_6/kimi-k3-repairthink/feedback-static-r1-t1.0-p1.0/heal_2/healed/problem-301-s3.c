#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_KEY_LENGTH 1024

typedef struct Node {
    char *key;
    int is_dict;
    struct Node **children;
    size_t child_count;
    size_t capacity;
} Node;

Node *create_node(const char *key, int is_dict) {
    if (key == NULL) {
        return NULL;
    }
    size_t key_length = strnlen(key, MAX_KEY_LENGTH);
    if (key_length >= MAX_KEY_LENGTH) {
        return NULL;
    }
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    size_t key_buffer_size = key_length + 1;
    node->key = malloc(key_buffer_size);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    int written = snprintf(node->key, key_buffer_size, "%s", key);
    if (written < 0 || (size_t)written >= key_buffer_size) {
        free(node->key);
        free(node);
        return NULL;
    }
    node->is_dict = is_dict;
    node->children = NULL;
    node->child_count = 0;
    node->capacity = 0;
    return node;
}

int add_child(Node *parent, Node *child) {
    if (parent == NULL || child == NULL) {
        return -1;
    }
    if (parent->child_count >= parent->capacity) {
        if (parent->capacity > (SIZE_MAX / sizeof(Node *)) / 2) {
            return -1;
        }
        size_t new_capacity = parent->capacity == 0 ? 4 : parent->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(Node *)) {
            return -1;
        }
        Node **new_children = realloc(parent->children, new_capacity * sizeof(Node *));
        if (new_children == NULL) {
            return -1;
        }
        parent->children = new_children;
        parent->capacity = new_capacity;
    }
    parent->children[parent->child_count++] = child;
    return 0;
}

void free_node(Node *node) {
    if (node == NULL) {
        return;
    }
    for (size_t i = 0; i < node->child_count; i++) {
        free_node(node->children[i]);
    }
    free(node->children);
    free(node->key);
    free(node);
}

int dict_depth(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    if (!node->is_dict) {
        return 0;
    }
    int max_depth = 0;
    for (size_t i = 0; i < node->child_count; i++) {
        int child_depth = dict_depth(node->children[i]);
        if (child_depth > max_depth) {
            max_depth = child_depth;
        }
    }
    return max_depth + 1;
}

int main(void) {
    Node *root = create_node("root", 1);
    if (root == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    Node *level1 = create_node("level1", 1);
    if (level1 == NULL) {
        free_node(root);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    if (add_child(root, level1) != 0) {
        free_node(level1);
        free_node(root);
        fprintf(stderr, "Failed to add child\n");
        return EXIT_FAILURE;
    }

    Node *level2 = create_node("level2", 1);
    if (level2 == NULL) {
        free_node(root);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    if (add_child(level1, level2) != 0) {
        free_node(level2);
        free_node(root);
        fprintf(stderr, "Failed to add child\n");
        return EXIT_FAILURE;
    }

    Node *level3 = create_node("level3", 1);
    if (level3 == NULL) {
        free_node(root);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    if (add_child(level2, level3) != 0) {
        free_node(level3);
        free_node(root);
        fprintf(stderr, "Failed to add child\n");
        return EXIT_FAILURE;
    }

    Node *leaf = create_node("leaf", 0);
    if (leaf == NULL) {
        free_node(root);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    if (add_child(level3, leaf) != 0) {
        free_node(leaf);
        free_node(root);
        fprintf(stderr, "Failed to add child\n");
        return EXIT_FAILURE;
    }

    int depth = dict_depth(root);
    printf("Dictionary depth: %d\n", depth);

    free_node(root);
    return EXIT_SUCCESS;
}