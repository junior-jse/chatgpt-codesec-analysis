#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node **children;
    int child_count;
} Node;

int *extract_column(Node *root, int column, int *result_size) {
    if (root == NULL || column < 0) {
        *result_size = 0;
        return NULL;
    }

    int *result = NULL;
    int count = 0;
    Node **current_level = (Node **)malloc(sizeof(Node *));
    if (current_level == NULL) {
        *result_size = 0;
        return NULL;
    }
    current_level[0] = root;
    int level_size = 1;

    while (level_size > 0) {
        if (column < level_size && current_level[column] != NULL) {
            int *new_result = (int *)realloc(result, (count + 1) * sizeof(int));
            if (new_result == NULL) {
                free(result);
                free(current_level);
                *result_size = 0;
                return NULL;
            }
            result = new_result;
            result[count] = current_level[column]->value;
            count++;
        } else {
            break;
        }

        int next_level_size = 0;
        for (int i = 0; i < level_size; i++) {
            if (current_level[i] != NULL) {
                next_level_size += current_level[i]->child_count;
            }
        }

        if (next_level_size == 0) {
            break;
        }

        Node **next_level = (Node **)malloc(next_level_size * sizeof(Node *));
        if (next_level == NULL) {
            free(result);
            free(current_level);
            *result_size = 0;
            return NULL;
        }

        int index = 0;
        for (int i = 0; i < level_size; i++) {
            if (current_level[i] != NULL) {
                for (int j = 0; j < current_level[i]->child_count; j++) {
                    next_level[index++] = current_level[i]->children[j];
                }
            }
        }

        free(current_level);
        current_level = next_level;
        level_size = next_level_size;
    }

    free(current_level);
    *result_size = count;
    return result;
}

Node *create_node(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

int add_child(Node *parent, Node *child) {
    Node **new_children = (Node **)realloc(parent->children, (parent->child_count + 1) * sizeof(Node *));
    if (new_children == NULL) {
        return -1;
    }
    parent->children = new_children;
    parent->children[parent->child_count] = child;
    parent->child_count++;
    return 0;
}

void free_tree(Node *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < root->child_count; i++) {
        free_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}

int main() {
    Node *root = create_node(1);
    Node *child1 = create_node(2);
    Node *child2 = create_node(3);
    Node *child3 = create_node(4);
    Node *grandchild1 = create_node(5);
    Node *grandchild2 = create_node(6);
    Node *grandchild3 = create_node(7);
    Node *grandchild4 = create_node(8);

    add_child(root, child1);
    add_child(root, child2);
    add_child(root, child3);
    add_child(child1, grandchild1);
    add_child(child1, grandchild2);
    add_child(child2, grandchild3);
    add_child(child3, grandchild4);

    int size;
    int *column_values = extract_column(root, 1, &size);

    if (column_values != NULL) {
        for (int i = 0; i < size; i++) {
            printf("%d ", column_values[i]);
        }
        printf("\n");
        free(column_values);
    }

    free_tree(root);
    return 0;
}