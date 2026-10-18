#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 4096

typedef struct DictNode {
    char *key;
    struct DictNode *value;
    struct DictNode *next;
} DictNode;

typedef struct {
    DictNode *head;
} Dict;

DictNode *create_node(const char *key, size_t len) {
    DictNode *node = (DictNode *)malloc(sizeof(DictNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = (char *)malloc(len + 1);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->key, key, len);
    node->key[len] = '\0';
    node->value = NULL;
    node->next = NULL;
    return node;
}

DictNode *find_key(DictNode *head, const char *key) {
    DictNode *current = head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_dict(DictNode *node) {
    while (node != NULL) {
        DictNode *next = node->next;
        free(node->key);
        if (node->value != NULL) {
            free_dict(node->value);
        }
        free(node);
        node = next;
    }
}

Dict *convert_lists_to_dict(char ***lists, int num_lists, const int *list_lengths) {
    Dict *dict = (Dict *)malloc(sizeof(Dict));
    if (dict == NULL) {
        return NULL;
    }
    dict->head = NULL;

    for (int i = 0; i < num_lists; i++) {
        if (list_lengths[i] <= 0) {
            free_dict(dict->head);
            free(dict);
            return NULL;
        }
        DictNode **current_level = &(dict->head);
        for (int j = 0; j < list_lengths[i] - 1; j++) {
            size_t key_len = strnlen(lists[i][j], MAX_STRING_LEN);
            if (key_len >= MAX_STRING_LEN) {
                free_dict(dict->head);
                free(dict);
                return NULL;
            }
            DictNode *existing = find_key(*current_level, lists[i][j]);
            if (existing == NULL) {
                existing = create_node(lists[i][j], key_len);
                if (existing == NULL) {
                    free_dict(dict->head);
                    free(dict);
                    return NULL;
                }
                existing->next = *current_level;
                *current_level = existing;
            }
            current_level = &(existing->value);
        }
        size_t leaf_len = strnlen(lists[i][list_lengths[i] - 1], MAX_STRING_LEN);
        if (leaf_len >= MAX_STRING_LEN) {
            free_dict(dict->head);
            free(dict);
            return NULL;
        }
        DictNode *leaf = find_key(*current_level, lists[i][list_lengths[i] - 1]);
        if (leaf == NULL) {
            leaf = create_node(lists[i][list_lengths[i] - 1], leaf_len);
            if (leaf == NULL) {
                free_dict(dict->head);
                free(dict);
                return NULL;
            }
            leaf->next = *current_level;
            *current_level = leaf;
        }
    }

    return dict;
}

int main() {
    char *list1[] = {"a", "b", "c"};
    char *list2[] = {"a", "b", "d"};
    char *list3[] = {"x", "y"};
    char *list4[] = {"a", "e"};

    char **lists[] = {list1, list2, list3, list4};
    const int lengths[] = {3, 3, 2, 2};

    Dict *result = convert_lists_to_dict(lists, 4, lengths);

    if (result != NULL) {
        printf("Dictionary created successfully\n");
        free_dict(result->head);
        free(result);
    } else {
        printf("Failed to create dictionary\n");
    }

    return 0;
}