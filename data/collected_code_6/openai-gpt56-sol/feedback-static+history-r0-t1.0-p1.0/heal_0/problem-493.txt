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
    size_t rows;
    size_t columns;
    size_t count;
    Hexagon *hexagons;
} HexagonGrid;

static void free_hexagon_grid(HexagonGrid *grid)
{
    if (grid == NULL) {
        return;
    }

    free(grid->hexagons);
    grid->hexagons = NULL;
    grid->rows = 0U;
    grid->columns = 0U;
    grid->count = 0U;
}

static int calculate_hexagon_grid(size_t rows, size_t columns,
                                  double radius, double origin_x,
                                  double origin_y, HexagonGrid *grid)
{
    static const double pi = 3.14159265358979323846;
    size_t count;
    double horizontal_spacing;
    double vertical_spacing;

    if (grid == NULL || rows == 0U || columns == 0U ||
        !isfinite(radius) || radius <= 0.0 ||
        !isfinite(origin_x) || !isfinite(origin_y)) {
        return EINVAL;
    }

    grid->rows = 0U;
    grid->columns = 0U;
    grid->count = 0U;
    grid->hexagons = NULL;

    if (columns > SIZE_MAX / rows) {
        return EOVERFLOW;
    }

    count = rows * columns;

    if (count > SIZE_MAX / sizeof(*grid->hexagons)) {
        return EOVERFLOW;
    }

    grid->hexagons = calloc(count, sizeof(*grid->hexagons));
    if (grid->hexagons == NULL) {
        return ENOMEM;
    }

    horizontal_spacing = sqrt(3.0) * radius;
    vertical_spacing = 1.5 * radius;

    if (!isfinite(horizontal_spacing) || !isfinite(vertical_spacing)) {
        free_hexagon_grid(grid);
        return ERANGE;
    }

    for (size_t row = 0U; row < rows; ++row) {
        for (size_t column = 0U; column < columns; ++column) {
            size_t index = row * columns + column;
            double center_x = origin_x + horizontal_spacing *
                ((double)column + ((row & 1U) != 0U ? 0.5 : 0.0));
            double center_y = origin_y + vertical_spacing * (double)row;

            if (!isfinite(center_x) || !isfinite(center_y)) {
                free_hexagon_grid(grid);
                return ERANGE;
            }

            for (size_t vertex = 0U; vertex < 6U; ++vertex) {
                double angle = pi / 180.0 * (60.0 * (double)vertex - 30.0);
                double x = center_x + radius * cos(angle);
                double y = center_y + radius * sin(angle);

                if (!isfinite(x) || !isfinite(y)) {
                    free_hexagon_grid(grid);
                    return ERANGE;
                }

                grid->hexagons[index].vertices[vertex].x = x;
                grid->hexagons[index].vertices[vertex].y = y;
            }
        }
    }

    grid->rows = rows;
    grid->columns = columns;
    grid->count = count;
    return 0;
}

int main(void)
{
    HexagonGrid grid = {0U, 0U, 0U, NULL};
    int result = calculate_hexagon_grid(3U, 4U, 10.0, 0.0, 0.0, &grid);

    if (result != 0) {
        fprintf(stderr, "Failed to calculate hexagon grid: %s\n",
                result == EOVERFLOW ? "size overflow" :
                result == ENOMEM ? "memory allocation failure" :
                result == ERANGE ? "coordinate out of range" :
                "invalid argument");
        return EXIT_FAILURE;
    }

    if (printf("[\n") < 0) {
        free_hexagon_grid(&grid);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < grid.count; ++i) {
        if (printf("  [") < 0) {
            free_hexagon_grid(&grid);
            return EXIT_FAILURE;
        }

        for (size_t j = 0U; j < 6U; ++j) {
            if (printf("(%g, %g)%s",
                       grid.hexagons[i].vertices[j].x,
                       grid.hexagons[i].vertices[j].y,
                       j < 5U ? ", " : "") < 0) {
                free_hexagon_grid(&grid);
                return EXIT_FAILURE;
            }
        }

        if (printf("]%s\n", i + 1U < grid.count ? "," : "") < 0) {
            free_hexagon_grid(&grid);
            return EXIT_FAILURE;
        }
    }

    if (printf("]\n") < 0) {
        free_hexagon_grid(&grid);
        return EXIT_FAILURE;
    }

    free_hexagon_grid(&grid);
    return EXIT_SUCCESS;
}