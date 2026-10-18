#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* createNode(int data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int calculateHeight(struct TreeNode* root) {
    if (root == NULL) {
        return 0;
    }
    int leftHeight = calculateHeight(root->left);
    int rightHeight = calculateHeight(root->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

void freeTree(struct TreeNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    struct TreeNode* root = createNode(1);
    if (root == NULL) {
        return EXIT_FAILURE;
    }
    
    root->left = createNode(2);
    if (root->left == NULL) {
        free(root);
        return EXIT_FAILURE;
    }
    
    root->right = createNode(3);
    if (root->right == NULL) {
        free(root->left);
        free(root);
        return EXIT_FAILURE;
    }
    
    root->left->left = createNode(4);
    if (root->left->left == NULL) {
        free(root->right);
        free(root->left);
        free(root);
        return EXIT_FAILURE;
    }
    
    root->left->right = createNode(5);
    if (root->left->right == NULL) {
        free(root->left->left);
        free(root->right);
        free(root->left);
        free(root);
        return EXIT_FAILURE;
    }
    
    int height = calculateHeight(root);
    printf("Height of the binary tree: %d\n", height);
    
    freeTree(root);
    
    return EXIT_SUCCESS;
}