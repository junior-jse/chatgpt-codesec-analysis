#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node *create_node(int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

int check_balance(struct Node *root, int *height) {
    int left_height = 0;
    int right_height = 0;
    int left_balanced = 0;
    int right_balanced = 0;

    if (root == NULL) {
        *height = 0;
        return 1;
    }

    left_balanced = check_balance(root->left, &left_height);
    right_balanced = check_balance(root->right, &right_height);

    *height = (left_height > right_height ? left_height : right_height) + 1;

    if (abs(left_height - right_height) > 1) {
        return 0;
    }

    return left_balanced && right_balanced;
}

int is_balanced(struct Node *root) {
    int height = 0;
    return check_balance(root, &height);
}

void free_tree(struct Node *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    struct Node *root = NULL;
    struct Node *temp = NULL;

    root = create_node(1);
    if (root == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    temp = create_node(2);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_tree(root);
        return EXIT_FAILURE;
    }
    root->left = temp;

    temp = create_node(3);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_tree(root);
        return EXIT_FAILURE;
    }
    root->right = temp;

    temp = create_node(4);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_tree(root);
        return EXIT_FAILURE;
    }
    root->left->left = temp;

    temp = create_node(5);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_tree(root);
        return EXIT_FAILURE;
    }
    root->left->right = temp;

    temp = create_node(6);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_tree(root);
        return EXIT_FAILURE;
    }
    root->right->left = temp;

    if (is_balanced(root)) {
        printf("The binary tree is balanced.\n");
    } else {
        printf("The binary tree is not balanced.\n");
    }

    free_tree(root);
    return EXIT_SUCCESS;
}