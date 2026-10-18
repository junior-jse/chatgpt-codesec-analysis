#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t row;
    size_t col;
} Position;

typedef struct {
    Position *positions;
    size_t length;
    double average;
} Path;

static int find_max_average_path(const double *matrix, size_t n, Path *result)
{
    double *sums = NULL;
    size_t *lengths = NULL;
    ptrdiff_t *parents = NULL;
    Position *positions = NULL;
    size_t cells;
    size_t best_index;
    size_t i;

    if (matrix == NULL || result == NULL || n == 0 ||
        n > SIZE_MAX / n) {
        return -1;
    }

    cells = n * n;

    if (cells > SIZE_MAX / sizeof(*sums) ||
        cells > SIZE_MAX / sizeof(*lengths) ||
        cells > SIZE_MAX / sizeof(*parents) ||
        (2 * n - 1) > SIZE_MAX / sizeof(*positions)) {
        return -1;
    }

    sums = malloc(cells * sizeof(*sums));
    lengths = malloc(cells * sizeof(*lengths));
    parents = malloc(cells * sizeof(*parents));

    if (sums == NULL || lengths == NULL || parents == NULL) {
        free(sums);
        free(lengths);
        free(parents);
        return -1;
    }

    for (i = 0; i < cells; ++i) {
        if (!isfinite(matrix[i])) {
            free(sums);
            free(lengths);
            free(parents);
            return -1;
        }
    }

    sums[0] = matrix[0];
    lengths[0] = 1;
    parents[0] = -1;

    for (i = 1; i < n; ++i) {
        size_t index = i;
        sums[index] = sums[index - 1] + matrix[index];
        lengths[index] = lengths[index - 1] + 1;
        parents[index] = (ptrdiff_t)(index - 1);
    }

    for (i = 1; i < n; ++i) {
        size_t index = i * n;
        sums[index] = sums[index - n] + matrix[index];
        lengths[index] = lengths[index - n] + 1;
        parents[index] = (ptrdiff_t)(index - n);
    }

    for (size_t row = 1; row < n; ++row) {
        for (size_t col = 1; col < n; ++col) {
            size_t index = row * n + col;
            size_t top = index - n;
            size_t left = index - 1;

            if (sums[top] > sums[left]) {
                sums[index] = sums[top] + matrix[index];
                lengths[index] = lengths[top] + 1;
                parents[index] = (ptrdiff_t)top;
            } else {
                sums[index] = sums[left] + matrix[index];
                lengths[index] = lengths[left] + 1;
                parents[index] = (ptrdiff_t)left;
            }
        }
    }

    best_index = cells - 1;
    positions = malloc(lengths[best_index] * sizeof(*positions));

    if (positions == NULL) {
        free(sums);
        free(lengths);
        free(parents);
        return -1;
    }

    {
        ptrdiff_t current = (ptrdiff_t)best_index;
        size_t position_index = lengths[best_index];

        while (current >= 0) {
            size_t index = (size_t)current;
            --position_index;
            positions[position_index].row = index / n;
            positions[position_index].col = index % n;
            current = parents[index];
        }
    }

    result->positions = positions;
    result->length = lengths[best_index];
    result->average = sums[best_index] / (double)lengths[best_index];

    free(sums);
    free(lengths);
    free(parents);
    return 0;
}

static void free_path(Path *path)
{
    if (path != NULL) {
        free(path->positions);
        path->positions = NULL;
        path->length = 0;
        path->average = 0.0;
    }
}

int main(void)
{
    size_t n;
    size_t cells;
    double *matrix = NULL;
    Path path = {NULL, 0, 0.0};

    if (scanf("%zu", &n) != 1 || n == 0 || n > SIZE_MAX / n) {
        fprintf(stderr, "Invalid matrix size\n");
        return EXIT_FAILURE;
    }

    cells = n * n;

    if (cells > SIZE_MAX / sizeof(*matrix)) {
        fprintf(stderr, "Matrix is too large\n");
        return EXIT_FAILURE;
    }

    matrix = malloc(cells * sizeof(*matrix));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < cells; ++i) {
        if (scanf("%lf", &matrix[i]) != 1 || !isfinite(matrix[i])) {
            fprintf(stderr, "Invalid matrix element\n");
            free(matrix);
            return EXIT_FAILURE;
        }
    }

    if (find_max_average_path(matrix, n, &path) != 0) {
        fprintf(stderr, "Unable to find path\n");
        free(matrix);
        return EXIT_FAILURE;
    }

    printf("%.10f\n", path.average);
    for (size_t i = 0; i < path.length; ++i) {
        printf("%zu %zu\n", path.positions[i].row, path.positions[i].col);
    }

    free_path(&path);
    free(matrix);
    return EXIT_SUCCESS;
}