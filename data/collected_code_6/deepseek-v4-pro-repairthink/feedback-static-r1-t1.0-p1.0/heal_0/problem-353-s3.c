#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node **rows;
    int num_rows;
    int num_cols;
} Matrix;

Node* create_node(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Matrix* create_matrix(int rows, int cols, int **data) {
    Matrix *matrix = (Matrix*)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }
    
    matrix->rows = (Node**)malloc(rows * sizeof(Node*));
    if (matrix->rows == NULL) {
        free(matrix);
        return NULL;
    }
    
    matrix->num_rows = rows;
    matrix->num_cols = cols;
    
    for (int i = 0; i < rows; i++) {
        matrix->rows[i] = NULL;
        Node *tail = NULL;
        for (int j = 0; j < cols; j++) {
            Node *node = create_node(data[i][j]);
            if (node == NULL) {
                for (int k = 0; k < i; k++) {
                    Node *current = matrix->rows[k];
                    while (current != NULL) {
                        Node *temp = current;
                        current = current->next;
                        free(temp);
                    }
                }
                free(matrix->rows);
                free(matrix);
                return NULL;
            }
            if (matrix->rows[i] == NULL) {
                matrix->rows[i] = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
        }
    }
    
    return matrix;
}

void free_matrix(Matrix *matrix) {
    if (matrix == NULL) {
        return;
    }
    
    for (int i = 0; i < matrix->num_rows; i++) {
        Node *current = matrix->rows[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(matrix->rows);
    free(matrix);
}

void remove_column(Matrix *matrix, int col_index) {
    if (matrix == NULL || matrix->rows == NULL) {
        return;
    }
    
    if (col_index < 0 || col_index >= matrix->num_cols) {
        return;
    }
    
    for (int i = 0; i < matrix->num_rows; i++) {
        Node *current = matrix->rows[i];
        Node *prev = NULL;
        int index = 0;
        
        while (current != NULL && index < col_index) {
            prev = current;
            current = current->next;
            index++;
        }
        
        if (current != NULL) {
            if (prev == NULL) {
                matrix->rows[i] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
        }
    }
    
    matrix->num_cols--;
}

void print_matrix(Matrix *matrix) {
    if (matrix == NULL) {
        return;
    }
    
    for (int i = 0; i < matrix->num_rows; i++) {
        Node *current = matrix->rows[i];
        while (current != NULL) {
            printf("%d ", current->data);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    int data[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    
    int *data_ptrs[3];
    for (int i = 0; i < 3; i++) {
        data_ptrs[i] = data[i];
    }
    
    Matrix *matrix = create_matrix(3, 4, data_ptrs);
    if (matrix == NULL) {
        fprintf(stderr, "Failed to create matrix\n");
        return 1;
    }
    
    printf("Original matrix:\n");
    print_matrix(matrix);
    
    remove_column(matrix, 1);
    
    printf("\nAfter removing column 1:\n");
    print_matrix(matrix);
    
    free_matrix(matrix);
    
    return 0;
}