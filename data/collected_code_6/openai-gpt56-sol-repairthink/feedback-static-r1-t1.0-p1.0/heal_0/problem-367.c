#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static void free_tree(TreeNode *root)
{
    if (root == NULL) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

static int balanced_height(const TreeNode *root)
{
    int left_height;
    int right_height;
    int difference;

    if (root == NULL) {
        return 0;
    }

    left_height = balanced_height(root->left);
    if (left_height < 0) {
        return -1;
    }

    right_height = balanced_height(root->right);
    if (right_height < 0) {
        return -1;
    }

    difference = left_height - right_height;
    if (difference < -1 || difference > 1) {
        return -1;
    }

    return 1 + (left_height > right_height ? left_height : right_height);
}

static bool is_balanced(const TreeNode *root)
{
    return balanced_height(root) >= 0;
}

int main(void)
{
    TreeNode *root = create_node(1);

    if (root == NULL) {
        fputs("Failed to allocate tree node.\n", stderr);
        return EXIT_FAILURE;
    }

    root->left = create_node(2);
    if (root->left == NULL) {
        fputs("Failed to allocate tree node.\n", stderr);
        free_tree(root);
        return EXIT_FAILURE;
    }

    root->right = create_node(3);
    if (root->right == NULL) {
        fputs("Failed to allocate tree node.\n", stderr);
        free_tree(root);
        return EXIT_FAILURE;
    }

    root->left->left = create_node(4);
    if (root->left->left == NULL) {
        fputs("Failed to allocate tree node.\n", stderr);
        free_tree(root);
        return EXIT_FAILURE;
    }

    root->left->right = create_node(5);
    if (root->left->right == NULL) {
        fputs("Failed to allocate tree node.\n", stderr);
        free_tree(root);
        return EXIT_FAILURE;
    }

    puts(is_balanced(root) ? "The tree is balanced." : "The tree is not balanced.");

    free_tree(root);
    return EXIT_SUCCESS;
}