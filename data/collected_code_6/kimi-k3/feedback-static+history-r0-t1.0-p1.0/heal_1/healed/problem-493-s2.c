#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define HEXAGON_POINTS 6
#define PI 3.14159265358979323846

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point points[HEXAGON_POINTS];
} Hexagon;

Hexagon* create_hexagon_grid(int rows, int cols, double size, double start_x, double start_y) {
    if (rows <= 0 || cols <= 0 || size <= 0.0) {
        return NULL;
    }
    
    if (rows > INT_MAX / cols) {
        return NULL;
    }
    
    size_t total = (size_t)rows * (size_t)cols;
    Hexagon* grid = malloc(total * sizeof(Hexagon));
    
    if (grid == NULL) {
        return NULL;
    }
    
    double h_spacing = size * 1.5;
    double v_spacing = size * sqrt(3.0);
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            size_t idx = (size_t)row * (size_t)cols + (size_t)col;
            double cx = start_x + col * h_spacing;
            double cy = start_y + row * v_spacing;
            
            if (col % 2 == 1) {
                cy += v_spacing / 2.0;
            }
            
            for (int i = 0; i < HEXAGON_POINTS; i++) {
                double angle = PI / 3.0 * i + PI / 6.0;
                grid[idx].points[i].x = cx + size * cos(angle);
                grid[idx].points[i].y = cy + size * sin(angle);
            }
        }
    }
    
    return grid;
}

void destroy_hexagon_grid(Hexagon* grid) {
    free(grid);
}

int main(void) {
    int rows = 3;
    int cols = 4;
    double size = 10.0;
    
    Hexagon* grid = create_hexagon_grid(rows, cols, size, 0.0, 0.0);
    
    if (grid == NULL) {
        fprintf(stderr, "Failed to create hexagon grid\n");
        return EXIT_FAILURE;
    }
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            size_t idx = (size_t)row * (size_t)cols + (size_t)col;
            printf("Hexagon [%d][%d]:\n", row, col);
            for (int i = 0; i < HEXAGON_POINTS; i++) {
                printf("  Point %d: (%.2f, %.2f)\n", 
                       i, grid[idx].points[i].x, grid[idx].points[i].y);
            }
        }
    }
    
    destroy_hexagon_grid(grid);
    
    return EXIT_SUCCESS;
}