#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[6];
} Hexagon;

typedef struct {
    Hexagon *items;
    size_t rows;
    size_t columns;
    size_t count;
} HexagonGrid;

static void free_hexagon_grid(HexagonGrid *grid)
{
    if (grid == NULL) {
        return;
    }

    free(grid->items);
    grid->items = NULL;
    grid->rows = 0;
    grid->columns = 0;
    grid->count = 0;
}

static int calculate_hexagon_grid(size_t rows, size_t columns, double side_length,
                                  HexagonGrid *grid)
{
    const double pi = 3.14159265358979323846;
    double hex_height;
    size_t count;
    size_t row;
    size_t column;

    if (grid == NULL || rows == 0 || columns == 0 ||
        !isfinite(side_length) || side_length <= 0.0) {
        return 0;
    }

    grid->items = NULL;
    grid->rows = 0;
    grid->columns = 0;
    grid->count = 0;

    if (columns > SIZE_MAX / rows) {
        return 0;
    }

    count = rows * columns;

    if (count > SIZE_MAX / sizeof(*grid->items)) {
        return 0;
    }

    grid->items = calloc(count, sizeof(*grid->items));
    if (grid->items == NULL) {
        return 0;
    }

    hex_height = sqrt(3.0) * side_length;
    if (!isfinite(hex_height)) {
        free_hexagon_grid(grid);
        return 0;
    }

    for (row = 0; row < rows; ++row) {
        for (column = 0; column < columns; ++column) {
            Hexagon *hexagon = &grid->items[row * columns + column];
            double center_x = 1.5 * side_length * (double)column;
            double center_y = hex_height * ((double)row +
                                             0.5 * (double)(column % 2));
            size_t vertex;

            if (!isfinite(center_x) || !isfinite(center_y)) {
                free_hexagon_grid(grid);
                return 0;
            }

            for (vertex = 0; vertex < 6; ++vertex) {
                double angle = pi * (double)(60 * vertex) / 180.0;
                hexagon->vertices[vertex].x =
                    center_x + side_length * cos(angle);
                hexagon->vertices[vertex].y =
                    center_y + side_length * sin(angle);

                if (!isfinite(hexagon->vertices[vertex].x) ||
                    !isfinite(hexagon->vertices[vertex].y)) {
                    free_hexagon_grid(grid);
                    return 0;
                }
            }
        }
    }

    grid->rows = rows;
    grid->columns = columns;
    grid->count = count;
    return 1;
}

int main(void)
{
    HexagonGrid grid = {0};
    size_t index;
    size_t vertex;

    if (!calculate_hexagon_grid(3, 4, 10.0, &grid)) {
        fputs("Failed to calculate hexagon grid.\n", stderr);
        return EXIT_FAILURE;
    }

    putchar('[');
    for (index = 0; index < grid.count; ++index) {
        if (index != 0) {
            fputs(", ", stdout);
        }

        putchar('[');
        for (vertex = 0; vertex < 6; ++vertex) {
            const Point *point = &grid.items[index].vertices[vertex];

            if (vertex != 0) {
                fputs(", ", stdout);
            }

            if (printf("(%.6f, %.6f)", point->x, point->y) < 0) {
                free_hexagon_grid(&grid);
                return EXIT_FAILURE;
            }
        }
        putchar(']');
    }
    puts("]");

    if (ferror(stdout)) {
        free_hexagon_grid(&grid);
        return EXIT_FAILURE;
    }

    free_hexagon_grid(&grid);
    return EXIT_SUCCESS;
}