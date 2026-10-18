#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

double ***calculate_hex_grid(int rows, int cols, double size) {
    double x_offset = size * 2.0;
    double y_offset = size * sqrt(3.0);

    double ***grid = (double ***)malloc(rows * sizeof(double **));
    for (int i = 0; i < rows; i++) {
        grid[i] = (double **)malloc(cols * sizeof(double *));
        for (int j = 0; j < cols; j++) {
            grid[i][j] = (double *)malloc(12 * sizeof(double));
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double cx = j * x_offset + (i % 2) * (size * 1.5);
            double cy = i * y_offset * 0.5;

            for (int k = 0; k < 6; k++) {
                double angle_rad = M_PI / 180.0 * (60.0 * k - 30.0);
                grid[i][j][k * 2] = cx + size * cos(angle_rad);
                grid[i][j][k * 2 + 1] = cy + size * sin(angle_rad);
            }
        }
    }

    return grid;
}

void free_hex_grid(double ***grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            free(grid[i][j]);
        }
        free(grid[i]);
    }
    free(grid);
}

int main() {
    int rows = 3;
    int cols = 4;
    double size = 1.0;

    double ***grid = calculate_hex_grid(rows, cols, size);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Hexagon [%d][%d]: ", i, j);
            for (int k = 0; k < 6; k++) {
                printf("(%.2f, %.2f) ", grid[i][j][k * 2], grid[i][j][k * 2 + 1]);
            }
            printf("\n");
        }
    }

    free_hex_grid(grid, rows, cols);

    return 0;
}