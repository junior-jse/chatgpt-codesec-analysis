#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* create_node(int val) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int tree_height(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + max(tree_height(root->left), tree_height(root->right));
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    TreeNode *root = create_node(1);
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