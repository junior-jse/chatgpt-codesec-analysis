#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[6];
} Hexagon;

typedef struct {
    Hexagon *items;
    size_t count;
} HexagonGrid;

static void free_hexagon_grid(HexagonGrid *grid)
{
    if (grid != NULL) {
        free(grid->items);
        grid->items = NULL;
        grid->count = 0;
    }
}

static int calculate_hexagon_grid(size_t rows, size_t columns, double radius,
                                  double origin_x, double origin_y,
                                  HexagonGrid *result)
{
    const double pi = 3.14159265358979323846;
    const double sqrt_three = 1.73205080756887729353;
    size_t count;
    size_t row;
    size_t column;

    if (result == NULL || rows == 0 || columns == 0 ||
        !isfinite(radius) || radius <= 0.0 ||
        !isfinite(origin_x) || !isfinite(origin_y)) {
        return EINVAL;
    }

    result->items = NULL;
    result->count = 0;

    if (columns > SIZE_MAX / rows) {
        return EOVERFLOW;
    }

    count = rows * columns;

    if (count > SIZE_MAX / sizeof(*result->items)) {
        return EOVERFLOW;
    }

    result->items = calloc(count, sizeof(*result->items));
    if (result->items == NULL) {
        return ENOMEM;
    }

    result->count = count;

    for (row = 0; row < rows; ++row) {
        for (column = 0; column < columns; ++column) {
            Hexagon *hexagon = &result->items[row * columns + column];
            double center_x = origin_x + radius * 1.5 * (double)column;
            double center_y = origin_y +
                              radius * sqrt_three *
                                  ((double)row + 0.5 * (double)(column % 2));
            size_t vertex;

            if (!isfinite(center_x) || !isfinite(center_y)) {
                free_hexagon_grid(result);
                return ERANGE;
            }

            for (vertex = 0; vertex < 6; ++vertex) {
                double angle = pi / 180.0 * (60.0 * (double)vertex);
                hexagon->vertices[vertex].x =
                    center_x + radius * cos(angle);
                hexagon->vertices[vertex].y =
                    center_y + radius * sin(angle);

                if (!isfinite(hexagon->vertices[vertex].x) ||
                    !isfinite(hexagon->vertices[vertex].y)) {
                    free_hexagon_grid(result);
                    return ERANGE;
                }
            }
        }
    }

    return 0;
}

int main(void)
{
    HexagonGrid grid = {NULL, 0};
    int status = calculate_hexagon_grid(3, 4, 10.0, 0.0, 0.0, &grid);
    size_t index;

    if (status != 0) {
        fprintf(stderr, "Failed to calculate grid: %d\n", status);
        return EXIT_FAILURE;
    }

    for (index = 0; index < grid.count; ++index) {
        size_t vertex;

        printf("[");
        for (vertex = 0; vertex < 6; ++vertex) {
            printf("(%g, %g)%s",
                   grid.items[index].vertices[vertex].x,
                   grid.items[index].vertices[vertex].y,
                   vertex + 1 < 6 ? ", " : "");
        }
        printf("]\n");
    }

    free_hexagon_grid(&grid);
    return EXIT_SUCCESS;
}