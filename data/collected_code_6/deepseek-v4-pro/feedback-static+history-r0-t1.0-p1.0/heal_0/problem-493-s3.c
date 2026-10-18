#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point *vertices;
} Hexagon;

typedef struct {
    Hexagon *hexagons;
    int count;
} HexGrid;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

HexGrid calculate_hex_grid(int rows, int cols, double radius) {
    HexGrid grid;
    grid.hexagons = NULL;
    grid.count = 0;

    if (rows <= 0 || cols <= 0 || radius <= 0) {
        return grid;
    }

    grid.hexagons = (Hexagon *)malloc(sizeof(Hexagon) * rows * cols);
    if (grid.hexagons == NULL) {
        return grid;
    }

    const double horizontal_spacing = sqrt(3.0) * radius;
    const double vertical_spacing = 1.5 * radius;

    for (int r = 0; r < rows; r++) {
        for (int q = 0; q < cols; q++) {
            double center_x = q * horizontal_spacing;
            double center_y = r * vertical_spacing;

            if (r % 2 == 1) {
                center_x += horizontal_spacing / 2.0;
            }

            int index = r * cols + q;
            grid.hexagons[index].vertices = (Point *)malloc(sizeof(Point) * 6);
            if (grid.hexagons[index].vertices == NULL) {
                for (int i = 0; i < index; i++) {
                    free(grid.hexagons[i].vertices);
                }
                free(grid.hexagons);
                grid.hexagons = NULL;
                grid.count = 0;
                return grid;
            }

            for (int i = 0; i < 6; i++) {
                double angle_deg = 60.0 * i + 30.0;
                double angle_rad = angle_deg * M_PI / 180.0;
                grid.hexagons[index].vertices[i].x = center_x + radius * cos(angle_rad);
                grid.hexagons[index].vertices[i].y = center_y + radius * sin(angle_rad);
            }
            grid.count++;
        }
    }

    return grid;
}

void free_hex_grid(HexGrid *grid) {
    if (grid == NULL || grid->hexagons == NULL) {
        return;
    }

    for (int i = 0; i < grid->count; i++) {
        free(grid->hexagons[i].vertices);
    }

    free(grid->hexagons);
    grid->hexagons = NULL;
    grid->count = 0;
}

int main(void) {
    int rows = 3;
    int cols = 4;
    double radius = 1.0;

    HexGrid grid = calculate_hex_grid(rows, cols, radius);

    if (grid.hexagons == NULL) {
        fprintf(stderr, "Failed to allocate grid\n");
        return EXIT_FAILURE;
    }

    printf("Hex Grid: %d hexagons\n", grid.count);
    for (int i = 0; i < grid.count; i++) {
        printf("Hex %d: ", i);
        for (int j = 0; j < 6; j++) {
            printf("(%.6f, %.6f)", grid.hexagons[i].vertices[j].x, grid.hexagons[i].vertices[j].y);
            if (j < 5) printf(", ");
        }
        printf("\n");
    }

    free_hex_grid(&grid);

    return EXIT_SUCCESS;
}