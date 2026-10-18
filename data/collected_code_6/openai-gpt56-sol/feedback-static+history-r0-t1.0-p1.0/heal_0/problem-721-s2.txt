#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t row;
    size_t col;
} Point;

typedef struct {
    double sum;
    size_t length;
    size_t previous_col;
} State;

static int is_better(double sum_a, size_t length_a,
                     double sum_b, size_t length_b)
{
    long double left = (long double)sum_a * (long double)length_b;
    long double right = (long double)sum_b * (long double)length_a;

    if (left > right) {
        return 1;
    }

    if (left < right) {
        return 0;
    }

    return length_a < length_b;
}

static int find_maximum_average_path(const double *matrix, size_t n,
                                     Point **path, size_t *path_length,
                                     double *average)
{
    State *states = NULL;
    unsigned char *reachable = NULL;
    size_t cell_count;
    size_t best_index = 0;
    int found = 0;

    if (matrix == NULL || path == NULL || path_length == NULL ||
        average == NULL || n == 0 || n > SIZE_MAX / n) {
        return 0;
    }

    cell_count = n * n;

    if (cell_count > SIZE_MAX / sizeof(*states)) {
        return 0;
    }

    states = malloc(cell_count * sizeof(*states));
    reachable = calloc(cell_count, sizeof(*reachable));

    if (states == NULL || reachable == NULL) {
        free(states);
        free(reachable);
        return 0;
    }

    for (size_t start_col = 0; start_col < n; ++start_col) {
        size_t index = start_col;
        states[index].sum = matrix[index];
        states[index].length = 1;
        states[index].previous_col = SIZE_MAX;
        reachable[index] = 1;

        if (!found ||
            is_better(states[index].sum, states[index].length,
                      states[best_index].sum, states[best_index].length)) {
            best_index = index;
            found = 1;
        }
    }

    for (size_t row = 1; row < n; ++row) {
        for (size_t col = 0; col < n; ++col) {
            size_t index = row * n + col;
            size_t first_parent = col == 0 ? 0 : col - 1;
            size_t last_parent = col + 1 < n ? col + 1 : n - 1;

            for (size_t parent_col = first_parent;
                 parent_col <= last_parent;
                 ++parent_col) {
                size_t parent_index = (row - 1) * n + parent_col;
                double candidate_sum;
                size_t candidate_length;

                if (reachable[parent_index] == 0) {
                    continue;
                }

                candidate_sum = states[parent_index].sum + matrix[index];
                candidate_length = states[parent_index].length + 1;

                if (!isfinite(candidate_sum)) {
                    free(states);
                    free(reachable);
                    return 0;
                }

                if (reachable[index] == 0 ||
                    is_better(candidate_sum, candidate_length,
                              states[index].sum, states[index].length)) {
                    states[index].sum = candidate_sum;
                    states[index].length = candidate_length;
                    states[index].previous_col = parent_col;
                    reachable[index] = 1;
                }
            }

            if (reachable[index] != 0 &&
                is_better(states[index].sum, states[index].length,
                          states[best_index].sum, states[best_index].length)) {
                best_index = index;
            }
        }
    }

    *path_length = states[best_index].length;

    if (*path_length > SIZE_MAX / sizeof(**path)) {
        free(states);
        free(reachable);
        return 0;
    }

    *path = malloc(*path_length * sizeof(**path));

    if (*path == NULL) {
        free(states);
        free(reachable);
        return 0;
    }

    {
        size_t index = best_index;

        for (size_t position = *path_length; position > 0; --position) {
            size_t row = index / n;
            size_t col = index % n;

            (*path)[position - 1].row = row;
            (*path)[position - 1].col = col;

            if (position > 1) {
                index = (row - 1) * n + states[index].previous_col;
            }
        }
    }

    *average = states[best_index].sum / (double)states[best_index].length;

    free(states);
    free(reachable);
    return 1;
}

int main(void)
{
    size_t n;
    size_t cell_count;
    double *matrix = NULL;
    Point *path = NULL;
    size_t path_length = 0;
    double average;

    if (scanf("%zu", &n) != 1 || n == 0 || n > SIZE_MAX / n) {
        fprintf(stderr, "Invalid matrix size.\n");
        return EXIT_FAILURE;
    }

    cell_count = n * n;

    if (cell_count > SIZE_MAX / sizeof(*matrix)) {
        fprintf(stderr, "Matrix is too large.\n");
        return EXIT_FAILURE;
    }

    matrix = malloc(cell_count * sizeof(*matrix));

    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < cell_count; ++i) {
        errno = 0;

        if (scanf("%lf", &matrix[i]) != 1 ||
            errno == ERANGE || !isfinite(matrix[i])) {
            fprintf(stderr, "Invalid matrix element.\n");
            free(matrix);
            return EXIT_FAILURE;
        }
    }

    if (!find_maximum_average_path(matrix, n, &path, &path_length, &average)) {
        fprintf(stderr, "Unable to find a path.\n");
        free(matrix);
        return EXIT_FAILURE;
    }

    printf("%.17g\n", average);

    for (size_t i = 0; i < path_length; ++i) {
        printf("%zu %zu\n", path[i].row, path[i].col);
    }

    free(path);
    free(matrix);
    return EXIT_SUCCESS;
}