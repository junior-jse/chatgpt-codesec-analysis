#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_SIZE 1
#define MAX_SIZE 99

int** allocate_square(int n) {
    int **square = NULL;
    int i;
    
    if (n < MIN_SIZE || n > MAX_SIZE || n % 2 == 0) {
        return NULL;
    }
    
    square = (int**)malloc((size_t)n * sizeof(int*));
    if (square == NULL) {
        return NULL;
    }
    
    for (i = 0; i < n; i++) {
        square[i] = (int*)calloc((size_t)n, sizeof(int));
        if (square[i] == NULL) {
            int j;
            for (j = 0; j < i; j++) {
                free(square[j]);
                square[j] = NULL;
            }
            free(square);
            square = NULL;
            return NULL;
        }
    }
    
    return square;
}

void free_square(int **square, int n) {
    int i;
    
    if (square == NULL) {
        return;
    }
    
    for (i = 0; i < n; i++) {
        free(square[i]);
        square[i] = NULL;
    }
    free(square);
    square = NULL;
}

int generate_magic_square(int **square, int n) {
    int i, j, num;
    int row, col, next_row, next_col;
    
    if (square == NULL || n < MIN_SIZE || n > MAX_SIZE || n % 2 == 0) {
        return 0;
    }
    
    row = 0;
    col = n / 2;
    
    for (num = 1; num <= n * n; num++) {
        square[row][col] = num;
        
        next_row = row - 1;
        next_col = col + 1;
        
        if (next_row < 0) {
            next_row = n - 1;
        }
        if (next_col >= n) {
            next_col = 0;
        }
        
        if (square[next_row][next_col] != 0) {
            next_row = row + 1;
            next_col = col;
        }
        
        row = next_row;
        col = next_col;
    }
    
    return 1;
}

int verify_magic_square(int **square, int n) {
    int magic_sum;
    int i, j;
    int sum;
    
    if (square == NULL || n < MIN_SIZE || n > MAX_SIZE) {
        return 0;
    }
    
    magic_sum = n * (n * n + 1) / 2;
    
    for (i = 0; i < n; i++) {
        sum = 0;
        for (j = 0; j < n; j++) {
            sum += square[i][j];
        }
        if (sum != magic_sum) {
            return 0;
        }
    }
    
    for (j = 0; j < n; j++) {
        sum = 0;
        for (i = 0; i < n; i++) {
            sum += square[i][j];
        }
        if (sum != magic_sum) {
            return 0;
        }
    }
    
    sum = 0;
    for (i = 0; i < n; i++) {
        sum += square[i][i];
    }
    if (sum != magic_sum) {
        return 0;
    }
    
    sum = 0;
    for (i = 0; i < n; i++) {
        sum += square[i][n - 1 - i];
    }
    if (sum != magic_sum) {
        return 0;
    }
    
    return 1;
}

void print_magic_square(int **square, int n) {
    int i, j;
    
    if (square == NULL || n < MIN_SIZE || n > MAX_SIZE) {
        return;
    }
    
    printf("Magic Square of size %d:\n", n);
    printf("Magic constant: %d\n\n", n * (n * n + 1) / 2);
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%4d ", square[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    int n;
    int **square = NULL;
    int result;
    
    printf("Enter odd size of magic square (%d-%d): ", MIN_SIZE, MAX_SIZE);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (n < MIN_SIZE || n > MAX_SIZE) {
        fprintf(stderr, "Error: Size must be between %d and %d\n", MIN_SIZE, MAX_SIZE);
        return EXIT_FAILURE;
    }
    
    if (n % 2 == 0) {
        fprintf(stderr, "Error: Size must be odd\n");
        return EXIT_FAILURE;
    }
    
    square = allocate_square(n);
    if (square == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    result = generate_magic_square(square, n);
    if (!result) {
        fprintf(stderr, "Error: Failed to generate magic square\n");
        free_square(square, n);
        return EXIT_FAILURE;
    }
    
    print_magic_square(square, n);
    
    if (verify_magic_square(square, n)) {
        printf("Verification: Valid magic square\n");
    } else {
        printf("Verification: Invalid magic square\n");
    }
    
    free_square(square, n);
    square = NULL;
    
    return EXIT_SUCCESS;
}