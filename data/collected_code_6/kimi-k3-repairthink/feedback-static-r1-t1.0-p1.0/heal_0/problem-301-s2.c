#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct DictNode {
    char *key;
    int is_dict;
    struct DictNode *value;
    struct DictNode *next;
} DictNode;

DictNode *create_node(const char *key, int is_dict) {
    DictNode *node = malloc(sizeof(DictNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = malloc(strlen(key) + 1);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    strcpy(node->key, key);
    node->is_dict = is_dict;
    node->value = NULL;
    node->next = NULL;
    return node;
}

int add_pair(DictNode *dict, const char *key, DictNode *value, int is_dict) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    DictNode *node = create_node(key, is_dict);
    if (node == NULL) {
        return -1;
    }
    node->value = value;
    while (dict->next != NULL) {
        dict = dict->next;
    }
    dict->next = node;
    return 0;
}

int dict_depth(const DictNode *dict) {
    if (dict == NULL) {
        return 0;
    }
    int max_depth = 0;
    const DictNode *current = dict->next;
    while (current != NULL) {
        if (current->is_dict && current->value != NULL) {
            int depth = dict_depth(current->value);
            if (depth > max_depth) {
                max_depth = depth;
            }
        }
        current = current->next;
    }
    return max_depth + 1;
}

void free_dict(DictNode *dict) {
    if (dict == NULL) {
        return;
    }
    DictNode *current = dict->next;
    while (current != NULL) {
        DictNode *next = current->next;
        if (current->is_dict && current->value != NULL) {
            free_dict(current->value);
        }
        free(current->key);
        free(current);
        current = next;
    }
    free(dict->key);
    free(dict);
}

int main(void) {
    DictNode *root = create_node("root", 1);
    if (root == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    DictNode *level1 = create_node("level1", 1);
    if (level1 == NULL) {
        free_dict(root);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    DictNode *level2 = create_node("level2", 1);
    if (level2 == NULL) {
        free_dict(level1);
        free_dict(root);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    if (add_pair(root, "a", level1, 1) != 0) {
        free_dict(level2);
        free_dict(level1);
        free_dict(root);
        fprintf(stderr, "Failed to add pair\n");
        return EXIT_FAILURE;
    }

    if (add_pair(level1, "b", level2, 1) != 0) {
        free_dict(level2);
        free_dict(root);
        fprintf(stderr, "Failed to add pair\n");
        return EXIT_FAILURE;
    }

    if (add_pair(level2, "c", NULL, 0) != 0) {
        free_dict(root);
        fprintf(stderr, "Failed to add pair\n");
        return EXIT_FAILURE;
    }

    if (add_pair(root, "d", NULL, 0) != 0) {
        free_dict(root);
        fprintf(stderr, "Failed to add pair\n");
        return EXIT_FAILURE;
    }

    int depth = dict_depth(root);
    printf("Dictionary depth: %d\n", depth);

    free_dict(root);
    return EXIT_SUCCESS;
}