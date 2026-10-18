#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

static TreeNode *create_node(int value)
{
    TreeNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static size_t tree_height(const TreeNode *root)
{
    if (root == NULL) {
        return 0U;
    }

    size_t left_height = tree_height(root->left);
    size_t right_height = tree_height(root->right);

    return 1U + (left_height > right_height ? left_height : right_height);
}

static void free_tree(TreeNode *root)
{
    if (root == NULL) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void)
{
    TreeNode *root = create_node(1);

    if (root == NULL) {
        return EXIT_FAILURE;
    }

    root->left = create_node(2);
    if (root->left == NULL) {
        free_tree(root);
        return EXIT_FAILURE;
    }

    root->right = create_node(3);
    if (root->right == NULL) {
        free_tree(root);
        return EXIT_FAILURE;
    }

    root->left->left = create_node(4);
    if (root->left->left == NULL) {
        free_tree(root);
        return EXIT_FAILURE;
    }

    root->left->right = create_node(5);
    if (root->left->right == NULL) {
        free_tree(root);
        return EXIT_FAILURE;
    }

    printf("%zu\n", tree_height(root));
    free_tree(root);

    return EXIT_SUCCESS;
}