#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    NODE_INTEGER,
    NODE_LIST
} NodeType;

typedef struct Node Node;

struct Node {
    NodeType type;
    union {
        int value;
        struct {
            Node **items;
            size_t length;
        } list;
    } data;
};

typedef struct {
    int *values;
    size_t length;
    size_t capacity;
} FlatList;

static Node *node_create_integer(int value)
{
    Node *node = calloc(1, sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->type = NODE_INTEGER;
    node->data.value = value;
    return node;
}

static Node *node_create_list(size_t length)
{
    Node *node;

    if (length > SIZE_MAX / sizeof(Node *)) {
        return NULL;
    }

    node = calloc(1, sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->type = NODE_LIST;
    node->data.list.length = length;

    if (length != 0U) {
        node->data.list.items = calloc(length, sizeof(*node->data.list.items));
        if (node->data.list.items == NULL) {
            free(node);
            return NULL;
        }
    }

    return node;
}

static bool node_list_set(Node *list, size_t index, Node *item)
{
    if (list == NULL || list->type != NODE_LIST ||
        index >= list->data.list.length ||
        list->data.list.items[index] != NULL || item == NULL) {
        return false;
    }

    list->data.list.items[index] = item;
    return true;
}

static void node_destroy(Node *node)
{
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (node == NULL) {
        return;
    }

    if (node->type == NODE_LIST) {
        for (i = 0; i < node->data.list.length; ++i) {
            node_destroy(node->data.list.items[i]);
        }
        free(node->data.list.items);
    }

    free(node);
}

static void flat_list_destroy(FlatList *flat)
{
    if (flat == NULL) {
        return;
    }

    free(flat->values);
    flat->values = NULL;
    flat->length = 0U;
    flat->capacity = 0U;
}

static bool flat_list_append(FlatList *flat, int value)
{
    int *new_values;
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;

    if (flat == NULL) {
        return false;
    }

    if (flat->length == flat->capacity) {
        if (flat->capacity == 0U) {
            new_capacity = 8U;
        } else {
            if (flat->capacity > SIZE_MAX / 2U) {
                return false;
            }
            new_capacity = flat->capacity * 2U;
        }

        if (new_capacity > SIZE_MAX / sizeof(*flat->values)) {
            return false;
        }

        new_values = realloc(flat->values,
                             new_capacity * sizeof(*flat->values));
        if (new_values == NULL) {
            return false;
        }

        flat->values = new_values;
        flat->capacity = new_capacity;
    }

    flat->values[flat->length++] = value;
    return true;
}

static bool flatten_node(const Node *node, FlatList *flat)
{
    size_t i;

    if (node == NULL || flat == NULL) {
        return false;
    }

    switch (node->type) {
        case NODE_INTEGER:
            return flat_list_append(flat, node->data.value);

        case NODE_LIST:
            if (node->data.list.length != 0U &&
                node->data.list.items == NULL) {
                return false;
            }

            for (i = 0; i < node->data.list.length; ++i) {
                if (!flatten_node(node->data.list.items[i], flat)) {
                    return false;
                }
            }
            return true;

        default:
            return false;
    }
}

static bool flatten(const Node *root, FlatList *result)
{
    FlatList temporary = {0};

    if (root == NULL || result == NULL) {
        return false;
    }

    if (!flatten_node(root, &temporary)) {
        flat_list_destroy(&temporary);
        return false;
    }

    flat_list_destroy(result);
    *result = temporary;
    return true;
}

int main(void)
{
    Node *root = NULL;
    Node *inner = NULL;
    Node *deeper = NULL;
    Node *n1 = NULL;
    Node *n2 = NULL;
    Node *n3 = NULL;
    Node *n4 = NULL;
    Node *n5 = NULL;
    FlatList flat = {0};
    size_t i;
    int status = EXIT_FAILURE;

    root = node_create_list(3U);
    inner = node_create_list(2U);
    deeper = node_create_list(2U);
    n1 = node_create_integer(1);
    n2 = node_create_integer(2);
    n3 = node_create_integer(3);
    n4 = node_create_integer(4);
    n5 = node_create_integer(5);

    if (root == NULL || inner == NULL || deeper == NULL ||
        n1 == NULL || n2 == NULL || n3 == NULL ||
        n4 == NULL || n5 == NULL) {
        goto cleanup;
    }

    if (!node_list_set(deeper, 0U, n3)) {
        goto cleanup;
    }
    n3 = NULL;

    if (!node_list_set(deeper, 1U, n4)) {
        goto cleanup;
    }
    n4 = NULL;

    if (!node_list_set(inner, 0U, n2)) {
        goto cleanup;
    }
    n2 = NULL;

    if (!node_list_set(inner, 1U, deeper)) {
        goto cleanup;
    }
    deeper = NULL;

    if (!node_list_set(root, 0U, n1)) {
        goto cleanup;
    }
    n1 = NULL;

    if (!node_list_set(root, 1U, inner)) {
        goto cleanup;
    }
    inner = NULL;

    if (!node_list_set(root, 2U, n5)) {
        goto cleanup;
    }
    n5 = NULL;

    if (!flatten(root, &flat)) {
        goto cleanup;
    }

    for (i = 0; i < flat.length; ++i) {
        if (printf("%s%d", i == 0U ? "" : " ", flat.values[i]) < 0) {
            goto cleanup;
        }
    }

    if (putchar('\n') == EOF) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    flat_list_destroy(&flat);
    node_destroy(root);
    node_destroy(inner);
    node_destroy(deeper);
    node_destroy(n1);
    node_destroy(n2);
    node_destroy(n3);
    node_destroy(n4);
    node_destroy(n5);
    return status;
}