#include <stdio.h>
#include <stdlib.h>

void generate_magic_square(int n, int **magic) {
    /* Possible weaknesses found:
     *  The scope of the variable 'next_row' can be reduced. [variableScope]
     *  The scope of the variable 'next_col' can be reduced. [variableScope]
     */
    int row = 0, col = n / 2, num, next_row, next_col;

    for (num = 1; num <= n * n; num++) {
        magic[row][col] = num;
        next_row = (row - 1 + n) % n;
        next_col = (col + 1) % n;

        if (magic[next_row][next_col] != 0) {
            row = (row + 1) % n;
        } else {
            row = next_row;
            col = next_col;
        }
    }
}

int main(void) {
    int n, i, j;
    int **magic;

    printf("Enter odd number for magic square size: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (n <= 0 || n % 2 == 0) {
        fprintf(stderr, "Size must be a positive odd number\n");
        return EXIT_FAILURE;
    }

    magic = (int **)malloc(n * sizeof(int *));
    if (magic == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < n; i++) {
        magic[i] = (int *)calloc(n, sizeof(int));
        if (magic[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (j = 0; j < i; j++) {
                free(magic[j]);
            }
            free(magic);
            return EXIT_FAILURE;
        }
    }

    generate_magic_square(n, magic);

    printf("Magic Square of size %d:\n", n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%4d", magic[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < n; i++) {
        free(magic[i]);
    }
    free(magic);

    return EXIT_SUCCESS;
}