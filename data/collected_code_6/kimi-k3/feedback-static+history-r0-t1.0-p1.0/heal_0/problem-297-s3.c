#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_INT,
    NODE_LIST
} NodeType;

typedef struct Node Node;
struct Node {
    NodeType type;
    union {
        int value;
        struct {
            Node **items;
            size_t count;
            size_t capacity;
        } list;
    } data;
};

typedef struct {
    int *data;
    size_t count;
    size_t capacity;
} FlatList;

Node *node_create_int(int value)
{
    Node *node;

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->type = NODE_INT;
    node->data.value = value;
    return node;
}

Node *node_create_list(void)
{
    Node *node;

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->type = NODE_LIST;
    node->data.list.items = NULL;
    node->data.list.count = 0U;
    node->data.list.capacity = 0U;
    return node;
}

int node_list_append(Node *list, Node *item)
{
    Node **new_items;
    size_t new_capacity;

    if (list == NULL || item == NULL || list->type != NODE_LIST) {
        return -1;
    }

    if (list->data.list.count == list->data.list.capacity) {
        new_capacity = (list->data.list.capacity == 0U) ? 4U : list->data.list.capacity * 2U;
        new_items = realloc(list->data.list.items, new_capacity * sizeof(*new_items));
        if (new_items == NULL) {
            return -1;
        }
        list->data.list.items = new_items;
        list->data.list.capacity = new_capacity;
    }

    list->data.list.items[list->data.list.count] = item;
    list->data.list.count++;
    return 0;
}

void node_destroy(Node *node)
{
    size_t i;

    if (node == NULL) {
        return;
    }

    if (node->type == NODE_LIST) {
        for (i = 0U; i < node->data.list.count; i++) {
            node_destroy(node->data.list.items[i]);
        }
        free(node->data.list.items);
    }

    free(node);
}

int flat_list_append(FlatList *flat, int value)
{
    int *new_data;
    size_t new_capacity;

    if (flat == NULL) {
        return -1;
    }

    if (flat->count == flat->capacity) {
        new_capacity = (flat->capacity == 0U) ? 8U : flat->capacity * 2U;
        new_data = realloc(flat->data, new_capacity * sizeof(*new_data));
        if (new_data == NULL) {
            return -1;
        }
        flat->data = new_data;
        flat->capacity = new_capacity;
    }

    flat->data[flat->count] = value;
    flat->count++;
    return 0;
}

int flatten_node(const Node *node, FlatList *flat)
{
    size_t i;
    int result;

    if (node == NULL || flat == NULL) {
        return -1;
    }

    if (node->type == NODE_INT) {
        return flat_list_append(flat, node->data.value);
    }

    if (node->type == NODE_LIST) {
        for (i = 0U; i < node->data.list.count; i++) {
            result = flatten_node(node->data.list.items[i], flat);
            if (result != 0) {
                return result;
            }
        }
        return 0;
    }

    return -1;
}

FlatList *flatten(const Node *root)
{
    FlatList *flat;

    if (root == NULL) {
        return NULL;
    }

    flat = malloc(sizeof(*flat));
    if (flat == NULL) {
        return NULL;
    }

    flat->data = NULL;
    flat->count = 0U;
    flat->capacity = 0U;

    if (flatten_node(root, flat) != 0) {
        free(flat->data);
        free(flat);
        return NULL;
    }

    return flat;
}

void flat_list_destroy(FlatList *flat)
{
    if (flat == NULL) {
        return;
    }

    free(flat->data);
    free(flat);
}

int main(void)
{
    Node *root;
    Node *sub1;
    Node *sub2;
    FlatList *flat;
    size_t i;
    int status;

    root = node_create_list();
    if (root == NULL) {
        return EXIT_FAILURE;
    }

    sub1 = node_create_list();
    sub2 = node_create_list();
    if (sub1 == NULL || sub2 == NULL) {
        node_destroy(sub1);
        node_destroy(sub2);
        node_destroy(root);
        return EXIT_FAILURE;
    }

    status = 0;
    status |= node_list_append(root, node_create_int(1));
    status |= node_list_append(sub1, node_create_int(2));
    status |= node_list_append(sub1, node_create_int(3));
    status |= node_list_append(root, sub1);
    status |= node_list_append(sub2, node_create_int(4));
    status |= node_list_append(sub2, node_create_int(5));
    status |= node_list_append(root, sub2);
    status |= node_list_append(root, node_create_int(6));

    if (status != 0) {
        node_destroy(root);
        return EXIT_FAILURE;
    }

    flat = flatten(root);
    if (flat == NULL) {
        node_destroy(root);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < flat->count; i++) {
        printf("%d ", flat->data[i]);
    }
    printf("\n");

    flat_list_destroy(flat);
    node_destroy(root);
    return EXIT_SUCCESS;
}