/* Repair history (previous round):
 * - Flawfinder strcpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DictNode {
    char *key;
    struct DictNode *value;
    struct DictNode *next;
} DictNode;

typedef struct {
    DictNode *head;
} Dict;

DictNode *create_node(const char *key) {
    DictNode *node = (DictNode *)malloc(sizeof(DictNode));
    if (node == NULL) {
        return NULL;
    }
    size_t key_len = strnlen(key, 4096);
    node->key = (char *)malloc(key_len + 1);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(node->key, key, key_len);
    node->key[key_len] = '\0';
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
            free_dict(node->value->next);
            free(node->value);
        }
        free(node);
        node = next;
    }
}

/* Possible weaknesses found:
 *  expected 'char * const* const*' but argument is of type 'char ***'
 */
Dict *convert_lists_to_dict(char * const * const *lists, int num_lists, const int *list_lengths) {
    Dict *dict = (Dict *)malloc(sizeof(Dict));
    if (dict == NULL) {
        return NULL;
    }
    dict->head = NULL;

    for (int i = 0; i < num_lists; i++) {
        DictNode **current_level = &(dict->head);
        for (int j = 0; j < list_lengths[i] - 1; j++) {
            DictNode *existing = find_key(*current_level, lists[i][j]);
            if (existing == NULL) {
                existing = create_node(lists[i][j]);
                if (existing == NULL) {
                    free_dict(dict->head);
                    free(dict);
                    return NULL;
                }
                existing->next = *current_level;
                *current_level = existing;
            }
            if (existing->value == NULL) {
                existing->value = (DictNode *)malloc(sizeof(DictNode));
                if (existing->value == NULL) {
                    free_dict(dict->head);
                    free(dict);
                    return NULL;
                }
                existing->value->key = NULL;
                existing->value->value = NULL;
                existing->value->next = NULL;
            }
            current_level = &(existing->value);
        }
        DictNode *leaf = find_key(*current_level, lists[i][list_lengths[i] - 1]);
        if (leaf == NULL) {
            leaf = create_node(lists[i][list_lengths[i] - 1]);
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
    /* Possible weaknesses found:
     *  Variable 'lengths' can be declared as const array [constVariable]
     */
    int lengths[] = {3, 3, 2, 2};

    /* Possible weaknesses found:
     *  passing argument 1 of 'convert_lists_to_dict' from incompatible pointer type [-Wincompatible-pointer-types]
     */
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