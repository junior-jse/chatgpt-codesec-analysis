#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[6];
} Hexagon;

Hexagon* create_hexagon_grid(int rows, int cols, double radius) {
    if (rows <= 0 || cols <= 0 || radius <= 0.0) {
        return NULL;
    }

    Hexagon* grid = malloc((size_t)(rows * cols) * sizeof(Hexagon));
    if (grid == NULL) {
        return NULL;
    }

    double h_spacing = radius * 1.5;
    double v_spacing = radius * sqrt(3.0);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            double center_x = col * h_spacing;
            double center_y = row * v_spacing + (col % 2) * (v_spacing / 2.0);

            int index = row * cols + col;

            for (int i = 0; i < 6; i++) {
                double angle = M_PI / 3.0 * i;
                grid[index].vertices[i].x = center_x + radius * cos(angle);
                grid[index].vertices[i].y = center_y + radius * sin(angle);
            }
        }
    }

    return grid;
}

void free_hexagon_grid(Hexagon* grid) {
    free(grid);
}

int main(void) {
    int rows = 3;
    int cols = 3;
    double radius = 1.0;

    Hexagon* grid = create_hexagon_grid(rows, cols, radius);
    if (grid == NULL) {
        fprintf(stderr, "Failed to create hexagon grid\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < rows * cols; i++) {
        printf("Hexagon %d:\n", i);
        for (int j = 0; j < 6; j++) {
            printf("  Vertex %d: (%.2f, %.2f)\n", j, grid[i].vertices[j].x, grid[i].vertices[j].y);
        }
    }

    free_hexagon_grid(grid);
    return EXIT_SUCCESS;
}