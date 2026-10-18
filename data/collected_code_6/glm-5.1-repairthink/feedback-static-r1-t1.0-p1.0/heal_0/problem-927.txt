#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int tree_height(Node *root) {
    if (root == NULL) {
        return 0;
    }
    int left_height = tree_height(root->left);
    int right_height = tree_height(root->right);
    if (left_height > right_height) {
        return left_height + 1;
    } else {
        return right_height + 1;
    }
}

void free_tree(Node *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main() {
    Node *root = create_node(1);
    root->left = create_node(2);
    root->right = create_node(3);
    root->left->left = create_node(4);
    root->left->right = create_node(5);

    int height = tree_height(root);
    printf("%d\n", height);

    free_tree(root);
    root = NULL;

    return 0;
}