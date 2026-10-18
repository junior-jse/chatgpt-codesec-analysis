#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node *createNode(int data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int checkHeight(struct Node *root) {
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
    
    if (abs(leftHeight - rightHeight) > 1) {
        return -1;
    }
    
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

bool isBalanced(struct Node *root) {
    return checkHeight(root) != -1;
}

void freeTree(struct Node *root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    struct Node *root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    if (isBalanced(root)) {
        printf("Tree is balanced\n");
    } else {
        printf("Tree is not balanced\n");
    }
    
    freeTree(root);
    
    root = createNode(1);
    root->left = createNode(2);
    root->left->left = createNode(3);
    root->left->left->left = createNode(4);
    
    if (isBalanced(root)) {
        printf("Tree is balanced\n");
    } else {
        printf("Tree is not balanced\n");
    }
    
    freeTree(root);
    
    return 0;
}