#include <stdio.h>
#include <stdlib.h>

void generate_magic_square(int n, int **magic) {
    int i, j, num;
    int row, col;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            magic[i][j] = 0;
        }
    }

    row = 0;
    col = n / 2;

    for (num = 1; num <= n * n; num++) {
        magic[row][col] = num;
        row--;
        col++;

        if (num % n == 0) {
            row += 2;
            col--;
        } else {
            if (row < 0) {
                row = n - 1;
            }
            if (col == n) {
                col = 0;
            }
        }
    }
}

int **allocate_magic_square(int n) {
    int **magic;
    int i;

    magic = (int **)malloc(n * sizeof(int *));
    if (magic == NULL) {
        return NULL;
    }

    for (i = 0; i < n; i++) {
        magic[i] = (int *)malloc(n * sizeof(int));
        if (magic[i] == NULL) {
            int j;
            for (j = 0; j < i; j++) {
                free(magic[j]);
            }
            free(magic);
            return NULL;
        }
    }

    return magic;
}

void free_magic_square(int **magic, int n) {
    int i;
    if (magic == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        free(magic[i]);
    }
    free(magic);
}

void print_magic_square(int **magic, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%4d", magic[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n;
    int **magic_square;

    printf("Enter an odd number for the magic square size: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input.\n");
        return EXIT_FAILURE;
    }

    if (n <= 0 || n % 2 == 0) {
        fprintf(stderr, "Magic square requires a positive odd integer.\n");
        return EXIT_FAILURE;
    }

    magic_square = allocate_magic_square(n);
    if (magic_square == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    generate_magic_square(n, magic_square);
    print_magic_square(magic_square, n);

    free_magic_square(magic_square, n);

    return EXIT_SUCCESS;
}