#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { TYPE_INT, TYPE_DICT } ValueType;

typedef struct Dictionary {
    char *key;
    ValueType type;
    union {
        int int_val;
        struct Dictionary *dict_val;
    } value;
    struct Dictionary *next;
} Dictionary;

Dictionary *create_int_entry(const char *key, int val) {
    Dictionary *entry = (Dictionary *)malloc(sizeof(Dictionary));
    if (!entry) exit(EXIT_FAILURE);
    entry->key = strdup(key);
    if (!entry->key) exit(EXIT_FAILURE);
    entry->type = TYPE_INT;
    entry->value.int_val = val;
    entry->next = NULL;
    return entry;
}

Dictionary *create_dict_entry(const char *key, Dictionary *val) {
    Dictionary *entry = (Dictionary *)malloc(sizeof(Dictionary));
    if (!entry) exit(EXIT_FAILURE);
    entry->key = strdup(key);
    if (!entry->key) exit(EXIT_FAILURE);
    entry->type = TYPE_DICT;
    entry->value.dict_val = val;
    entry->next = NULL;
    return entry;
}

void free_dictionary(Dictionary *dict) {
    Dictionary *current = dict;
    while (current) {
        Dictionary *next = current->next;
        free(current->key);
        if (current->type == TYPE_DICT) {
            free_dictionary(current->value.dict_val);
        }
        free(current);
        current = next;
    }
}

int find_depth(Dictionary *dict) {
    if (!dict) {
        return 0;
    }
    int max_depth = 0;
    Dictionary *current = dict;
    while (current) {
        if (current->type == TYPE_DICT) {
            int depth = find_depth(current->value.dict_val);
            if (depth > max_depth) {
                max_depth = depth;
            }
        }
        current = current->next;
    }
    return 1 + max_depth;
}

int main(void) {
    Dictionary *inner_dict2 = create_int_entry("d", 40);
    Dictionary *inner_dict1 = create_int_entry("c", 30);
    inner_dict1->next = create_dict_entry("inner2", inner_dict2);
    
    Dictionary *root = create_int_entry("a", 10);
    root->next = create_dict_entry("inner1", inner_dict1);
    root->next->next = create_int_entry("b", 20);

    int depth = find_depth(root);
    printf("%d\n", depth);

    free_dictionary(root);
    return EXIT_SUCCESS;
}