#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct KeyValuePair {
    char *key;
    char *value;
    struct KeyValuePair *next;
} KeyValuePair;

typedef struct DictNode {
    char *key;
    struct DictNode *child;
    KeyValuePair *pairs;
    struct DictNode *next;
} DictNode;

typedef struct {
    DictNode *root;
    int depth;
} NestedDict;

static char *safe_strdup(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup != NULL) {
        memcpy(dup, str, len);
    }
    return dup;
}

static KeyValuePair *create_pair(const char *key, const char *value) {
    if (key == NULL || value == NULL) {
        return NULL;
    }
    KeyValuePair *pair = malloc(sizeof(KeyValuePair));
    if (pair == NULL) {
        return NULL;
    }
    pair->key = safe_strdup(key);
    pair->value = safe_strdup(value);
    pair->next = NULL;
    if (pair->key == NULL || pair->value == NULL) {
        free(pair->key);
        free(pair->value);
        free(pair);
        return NULL;
    }
    return pair;
}

static DictNode *create_dict_node(const char *key) {
    if (key == NULL) {
        return NULL;
    }
    DictNode *node = malloc(sizeof(DictNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = safe_strdup(key);
    node->child = NULL;
    node->pairs = NULL;
    node->next = NULL;
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    return node;
}

static void free_pairs(KeyValuePair *pairs) {
    while (pairs != NULL) {
        KeyValuePair *temp = pairs;
        pairs = pairs->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

static void free_dict_nodes(DictNode *nodes) {
    while (nodes != NULL) {
        DictNode *temp = nodes;
        nodes = nodes->next;
        free_dict_nodes(temp->child);
        free_pairs(temp->pairs);
        free(temp->key);
        free(temp);
    }
}

static NestedDict *create_nested_dict(void) {
    NestedDict *dict = malloc(sizeof(NestedDict));
    if (dict == NULL) {
        return NULL;
    }
    dict->root = NULL;
    dict->depth = 0;
    return dict;
}

static void free_nested_dict(NestedDict *dict) {
    if (dict != NULL) {
        free_dict_nodes(dict->root);
        free(dict);
    }
}

static DictNode *find_or_create_child(DictNode **parent, const char *key) {
    if (parent == NULL || key == NULL) {
        return NULL;
    }
    DictNode *current = *parent;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    DictNode *new_node = create_dict_node(key);
    if (new_node == NULL) {
        return NULL;
    }
    new_node->next = *parent;
    *parent = new_node;
    return new_node;
}

static bool add_pair_to_node(DictNode *node, const char *key, const char *value) {
    if (node == NULL || key == NULL || value == NULL) {
        return false;
    }
    KeyValuePair *pair = create_pair(key, value);
    if (pair == NULL) {
        return false;
    }
    pair->next = node->pairs;
    node->pairs = pair;
    return true;
}

static bool build_nested_level(DictNode **current, const char **keys, 
                               int num_keys, const char *value_key, 
                               const char *value) {
    if (current == NULL || keys == NULL || num_keys <= 0) {
        return false;
    }
    DictNode *node = find_or_create_child(current, keys[0]);
    if (node == NULL) {
        return false;
    }
    if (num_keys == 1) {
        return add_pair_to_node(node, value_key, value);
    }
    return build_nested_level(&node->child, keys + 1, num_keys - 1, 
                             value_key, value);
}

bool lists_to_nested_dict(NestedDict *dict, const char ***lists, 
                         const int *list_sizes, int num_lists,
                         const char *value_key) {
    if (dict == NULL || lists == NULL || list_sizes == NULL || 
        num_lists <= 0 || value_key == NULL) {
        return false;
    }
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || list_sizes[i] <= 0) {
            continue;
        }
        int size = list_sizes[i];
        const char *value = lists[i][size - 1];
        if (value == NULL) {
            continue;
        }
        if (size == 1) {
            if (!add_pair_to_node(dict->root, value_key, value)) {
                return false;
            }
        } else {
            if (!build_nested_level(&dict->root, lists[i], size - 1, 
                                   value_key, value)) {
                return false;
            }
        }
    }
    return true;
}

static void print_indent(int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
}

static void print_pairs(KeyValuePair *pairs, int indent) {
    while (pairs != NULL) {
        print_indent(indent);
        printf("\"%s\": \"%s\"\n", pairs->key, pairs->value);
        pairs = pairs->next;
    }
}

static void print_dict_nodes(DictNode *nodes, int indent) {
    while (nodes != NULL) {
        print_indent(indent);
        printf("\"%s\": {\n", nodes->key);
        print_pairs(nodes->pairs, indent + 1);
        print_dict_nodes(nodes->child, indent + 1);
        print_indent(indent);
        printf("}\n");
        nodes = nodes->next;
    }
}

void print_nested_dict(NestedDict *dict) {
    if (dict == NULL) {
        return;
    }
    printf("{\n");
    print_dict_nodes(dict->root, 1);
    printf("}\n");
}

int main(void) {
    NestedDict *dict = create_nested_dict();
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }

    const char *list1[] = {"USA", "California", "SanFrancisco", "Tech"};
    const char *list2[] = {"USA", "California", "LosAngeles", "Entertainment"};
    const char *list3[] = {"USA", "Texas", "Houston", "Energy"};
    const char *list4[] = {"Canada", "Ontario", "Toronto", "Finance"};
    const char *list5[] = {"Canada", "Quebec", "Montreal", "Aerospace"};

    const char **lists[] = {list1, list2, list3, list4, list5};
    int sizes[] = {4, 4, 4, 4, 4};

    if (!lists_to_nested_dict(dict, lists, sizes, 5, "Industry")) {
        fprintf(stderr, "Failed to build nested dictionary\n");
        free_nested_dict(dict);
        return EXIT_FAILURE;
    }

    print_nested_dict(dict);
    free_nested_dict(dict);

    return EXIT_SUCCESS;
}