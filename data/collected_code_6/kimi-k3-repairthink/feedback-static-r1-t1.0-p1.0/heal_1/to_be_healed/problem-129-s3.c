#include <stdio.h>
#include <stdlib.h>

int **allocate_magic_square(int n) {
    int **square = NULL;
    int i;
    
    if (n <= 0 || n % 2 == 0) {
        return NULL;
    }
    
    square = (int **)malloc(n * sizeof(int *));
    if (square == NULL) {
        return NULL;
    }
    
    for (i = 0; i < n; i++) {
        square[i] = (int *)calloc(n, sizeof(int));
        if (square[i] == NULL) {
            int j;
            for (j = 0; j < i; j++) {
                free(square[j]);
            }
            free(square);
            return NULL;
        }
    }
    
    return square;
}

void free_magic_square(int **square, int n) {
    int i;
    
    if (square == NULL) {
        return;
    }
    
    for (i = 0; i < n; i++) {
        if (square[i] != NULL) {
            free(square[i]);
        }
    }
    free(square);
}

int generate_magic_square(int **square, int n) {
    int i, j;
    /* Possible weaknesses found:
     *  The scope of the variable 'col' can be reduced. [variableScope]
     *  The scope of the variable 'row' can be reduced. [variableScope]
     */
    int row, col;
    int num;
    
    if (square == NULL || n <= 0 || n % 2 == 0) {
        return -1;
    }
    
    i = 0;
    j = n / 2;
    
    for (num = 1; num <= n * n; num++) {
        square[i][j] = num;
        
        row = (i - 1 + n) % n;
        col = (j + 1) % n;
        
        if (square[row][col] != 0) {
            row = (i + 1) % n;
            col = j;
        }
        
        i = row;
        j = col;
    }
    
    return 0;
}

void print_magic_square(int **square, int n) {
    int i, j;
    
    if (square == NULL || n <= 0) {
        return;
    }
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%4d", square[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n;
    int **magic_square = NULL;
    int result;
    
    printf("Enter an odd positive integer for magic square size: ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (n <= 0 || n % 2 == 0) {
        fprintf(stderr, "Error: Size must be a positive odd integer\n");
        return EXIT_FAILURE;
    }
    
    magic_square = allocate_magic_square(n);
    if (magic_square == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    result = generate_magic_square(magic_square, n);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to generate magic square\n");
        free_magic_square(magic_square, n);
        return EXIT_FAILURE;
    }
    
    printf("\nMagic Square of size %d:\n", n);
    print_magic_square(magic_square, n);
    
    printf("\nMagic constant: %d\n", n * (n * n + 1) / 2);
    
    free_magic_square(magic_square, n);
    
    return EXIT_SUCCESS;
}