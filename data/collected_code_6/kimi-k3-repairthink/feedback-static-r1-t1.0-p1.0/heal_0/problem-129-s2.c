#include <stdio.h>
#include <stdlib.h>

int** createMagicSquare(int n) {
    if (n <= 0 || n % 2 == 0) {
        return NULL;
    }
    
    int** square = (int**)malloc((size_t)n * sizeof(int*));
    if (square == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        square[i] = (int*)calloc((size_t)n, sizeof(int));
        if (square[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(square[j]);
            }
            free(square);
            return NULL;
        }
    }
    
    int num = 1;
    int i = 0;
    int j = n / 2;
    
    while (num <= n * n) {
        square[i][j] = num++;
        int newi = (i - 1 + n) % n;
        int newj = (j + 1) % n;
        
        if (square[newi][newj] != 0) {
            i = (i + 1) % n;
        } else {
            i = newi;
            j = newj;
        }
    }
    
    return square;
}

void freeMagicSquare(int** square, int n) {
    if (square == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        free(square[i]);
    }
    free(square);
}

void printMagicSquare(int** square, int n) {
    if (square == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", square[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n;
    
    printf("Enter odd number for magic square size: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (n <= 0 || n % 2 == 0) {
        fprintf(stderr, "Size must be a positive odd number\n");
        return EXIT_FAILURE;
    }
    
    int** square = createMagicSquare(n);
    if (square == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\nMagic Square (%dx%d):\n", n, n);
    printMagicSquare(square, n);
    printf("\nMagic constant: %d\n", n * (n * n + 1) / 2);
    
    freeMagicSquare(square, n);
    
    return EXIT_SUCCESS;
}