#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_POINTS 6
#define PI 3.14159265358979323846

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point points[NUM_POINTS];
} Hexagon;

typedef struct {
    Hexagon *data;
    size_t rows;
    size_t cols;
} HexagonGrid;

void free_hexagon_grid(HexagonGrid *grid) {
    if (grid != NULL) {
        free(grid->data);
        grid->data = NULL;
        grid->rows = 0;
        grid->cols = 0;
    }
}

int create_hexagon_grid(HexagonGrid *grid, size_t rows, size_t cols, double center_x, double center_y, double radius) {
    size_t i, j;
    int k;
    double angle, x_offset, y_offset;
    
    if (grid == NULL || rows == 0 || cols == 0 || radius <= 0.0) {
        return -1;
    }
    
    if (rows > SIZE_MAX / cols || rows * cols > SIZE_MAX / sizeof(Hexagon)) {
        return -1;
    }
    
    grid->data = (Hexagon *)malloc(rows * cols * sizeof(Hexagon));
    if (grid->data == NULL) {
        return -1;
    }
    
    grid->rows = rows;
    grid->cols = cols;
    
    x_offset = radius * 1.5;
    y_offset = radius * sqrt(3.0);
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            double cx = center_x + j * x_offset;
            double cy = center_y + i * y_offset;
            
            if (j % 2 == 1) {
                cy += y_offset / 2.0;
            }
            
            for (k = 0; k < NUM_POINTS; k++) {
                angle = 2.0 * PI * k / NUM_POINTS;
                grid->data[i * cols + j].points[k].x = cx + radius * cos(angle);
                grid->data[i * cols + j].points[k].y = cy + radius * sin(angle);
            }
        }
    }
    
    return 0;
}

int main(void) {
    HexagonGrid grid = {NULL, 0, 0};
    size_t rows = 3;
    size_t cols = 4;
    double center_x = 0.0;
    double center_y = 0.0;
    double radius = 10.0;
    size_t i, j;
    int k;
    
    if (create_hexagon_grid(&grid, rows, cols, center_x, center_y, radius) != 0) {
        fprintf(stderr, "Failed to create hexagon grid\n");
        return EXIT_FAILURE;
    }
    
    for (i = 0; i < grid.rows; i++) {
        for (j = 0; j < grid.cols; j++) {
            printf("Hexagon [%zu][%zu]:\n", i, j);
            for (k = 0; k < NUM_POINTS; k++) {
                printf("  Point %d: (%.2f, %.2f)\n", k, 
                       grid.data[i * grid.cols + j].points[k].x,
                       grid.data[i * grid.cols + j].points[k].y);
            }
        }
    }
    
    free_hexagon_grid(&grid);
    return EXIT_SUCCESS;
}