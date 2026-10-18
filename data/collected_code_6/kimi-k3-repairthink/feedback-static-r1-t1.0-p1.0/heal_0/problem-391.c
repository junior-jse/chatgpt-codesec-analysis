#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DictNode {
    char *key;
    char *value;
    struct DictNode *child;
    struct DictNode *next;
} DictNode;

DictNode *create_node(const char *key, const char *value) {
    if (key == NULL) {
        return NULL;
    }
    
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
    
    if (value != NULL) {
        node->value = malloc(strlen(value) + 1);
        if (node->value == NULL) {
            free(node->key);
            free(node);
            return NULL;
        }
        strcpy(node->value, value);
    } else {
        node->value = NULL;
    }
    
    node->child = NULL;
    node->next = NULL;
    return node;
}

void free_dict(DictNode *dict) {
    while (dict != NULL) {
        DictNode *next = dict->next;
        if (dict->child != NULL) {
            free_dict(dict->child);
        }
        free(dict->key);
        free(dict->value);
        free(dict);
        dict = next;
    }
}

int lists_to_nested_dict(char ***lists, size_t list_count, size_t list_length, DictNode **result) {
    if (lists == NULL || list_count == 0 || list_length == 0 || result == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < list_count; i++) {
        if (lists[i] == NULL) {
            return -1;
        }
        for (size_t j = 0; j < list_length; j++) {
            if (lists[i][j] == NULL) {
                return -1;
            }
        }
    }
    
    DictNode *root = create_node("root", NULL);
    if (root == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < list_length; i++) {
        DictNode *current = root;
        
        for (size_t j = 0; j < list_count; j++) {
            const char *key = lists[j][i];
            const char *value = (j == list_count - 1) ? lists[j][i] : NULL;
            
            DictNode *child = current->child;
            DictNode *prev = NULL;
            
            while (child != NULL && strcmp(child->key, key) != 0) {
                prev = child;
                child = child->next;
            }
            
            if (child == NULL) {
                child = create_node(key, value);
                if (child == NULL) {
                    free_dict(root);
                    return -1;
                }
                
                if (prev == NULL) {
                    current->child = child;
                } else {
                    prev->next = child;
                }
            } else if (value != NULL) {
                if (child->value != NULL) {
                    free(child->value);
                }
                child->value = malloc(strlen(value) + 1);
                if (child->value == NULL) {
                    free_dict(root);
                    return -1;
                }
                strcpy(child->value, value);
            }
            
            current = child;
        }
    }
    
    *result = root;
    return 0;
}

void print_dict(DictNode *dict, int depth) {
    while (dict != NULL) {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("%s", dict->key);
        if (dict->value != NULL) {
            printf(": %s", dict->value);
        }
        printf("\n");
        if (dict->child != NULL) {
            print_dict(dict->child, depth + 1);
        }
        dict = dict->next;
    }
}

int main(void) {
    char *names[] = {"Alice", "Bob", "Charlie"};
    char *cities[] = {"NewYork", "London", "Paris"};
    char *jobs[] = {"Engineer", "Doctor", "Artist"};
    
    char **lists[] = {names, cities, jobs};
    size_t list_count = 3;
    size_t list_length = 3;
    
    DictNode *result = NULL;
    
    if (lists_to_nested_dict(lists, list_count, list_length, &result) != 0) {
        fprintf(stderr, "Error: Failed to create nested dictionary\n");
        return EXIT_FAILURE;
    }
    
    printf("Nested Dictionary:\n");
    print_dict(result, 0);
    
    free_dict(result);
    return EXIT_SUCCESS;
}