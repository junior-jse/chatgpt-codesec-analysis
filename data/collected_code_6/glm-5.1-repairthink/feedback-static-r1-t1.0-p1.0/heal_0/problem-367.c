#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* createNode(int val) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int checkHeight(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = checkHeight(root->left);
    if (leftHeight == -1) {
        return -1;
    }

    int rightHeight = checkHeight(root->right);
    if (rightHeight == -1) {
        return -1;
    }

    int diff = leftHeight - rightHeight;
    if (diff > 1 || diff < -1) {
        return -1;
    }

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int isBalanced(TreeNode *root) {
    return checkHeight(root) != -1;
}

void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    TreeNode *root1 = createNode(1);
    root1->left = createNode(2);
    root1->right = createNode(3);
    root1->left->left = createNode(4);

    TreeNode *root2 = createNode(1);
    root2->left = createNode(2);
    root2->left->left = createNode(3);

    printf("%d\n", isBalanced(root1));
    printf("%d\n", isBalanced(root2));

    freeTree(root1);
    freeTree(root2);

    return 0;
}