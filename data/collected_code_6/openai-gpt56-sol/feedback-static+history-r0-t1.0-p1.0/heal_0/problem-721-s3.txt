#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>

typedef struct {
    size_t row;
    size_t col;
} Point;

typedef struct {
    double average;
    double sum;
    size_t length;
    Point *points;
} PathResult;

static bool add_size(size_t a, size_t b, size_t *result)
{
    if (a > SIZE_MAX - b) {
        return false;
    }
    *result = a + b;
    return true;
}

static bool multiply_size(size_t a, size_t b, size_t *result)
{
    if (a != 0U && b > SIZE_MAX / a) {
        return false;
    }
    *result = a * b;
    return true;
}

static bool find_maximum_average_path(const double *matrix,
                                      size_t n,
                                      PathResult *result)
{
    size_t cell_count;
    size_t path_capacity;
    size_t path_bytes;
    double best_average = -INFINITY;
    double best_sum = 0.0;
    size_t best_length = 0U;
    Point *best_path = NULL;

    if (matrix == NULL || result == NULL || n == 0U) {
        return false;
    }

    result->average = 0.0;
    result->sum = 0.0;
    result->length = 0U;
    result->points = NULL;

    if (!multiply_size(n, n, &cell_count) ||
        !add_size(n, n, &path_capacity) ||
        path_capacity == 0U) {
        return false;
    }

    --path_capacity;

    if (!multiply_size(path_capacity, sizeof(*best_path), &path_bytes)) {
        return false;
    }

    best_path = malloc(path_bytes);
    if (best_path == NULL) {
        return false;
    }

    for (size_t start_row = 0U; start_row < n; ++start_row) {
        for (size_t start_col = 0U; start_col < n; ++start_col) {
            for (size_t end_row = start_row; end_row < n; ++end_row) {
                for (size_t end_col = start_col; end_col < n; ++end_col) {
                    size_t length = (end_row - start_row) +
                                    (end_col - start_col) + 1U;
                    size_t vertical_steps = end_row - start_row;
                    size_t horizontal_steps = end_col - start_col;
                    size_t combinations = 1U;

                    for (size_t i = 1U; i <= horizontal_steps; ++i) {
                        size_t numerator = vertical_steps + i;

                        if (combinations > SIZE_MAX / numerator) {
                            free(best_path);
                            return false;
                        }

                        combinations = (combinations * numerator) / i;
                    }

                    for (size_t route = 0U; route < combinations; ++route) {
                        size_t rank = route;
                        size_t down_left = vertical_steps;
                        size_t right_left = horizontal_steps;
                        size_t row = start_row;
                        size_t col = start_col;
                        size_t index = 0U;
                        double sum = matrix[row * n + col];
                        Point temporary[path_capacity];

                        temporary[index++] = (Point){row, col};

                        while (down_left + right_left > 0U) {
                            size_t down_first_count = 0U;

                            if (down_left > 0U) {
                                size_t total_remaining = down_left + right_left - 1U;
                                size_t choose = right_left;
                                size_t value = 1U;

                                if (choose > total_remaining - choose) {
                                    choose = total_remaining - choose;
                                }

                                for (size_t i = 1U; i <= choose; ++i) {
                                    size_t numerator = total_remaining - choose + i;

                                    if (value > SIZE_MAX / numerator) {
                                        free(best_path);
                                        return false;
                                    }

                                    value = (value * numerator) / i;
                                }

                                down_first_count = value;
                            }

                            if (down_left > 0U && rank < down_first_count) {
                                ++row;
                                --down_left;
                            } else {
                                if (down_left > 0U) {
                                    rank -= down_first_count;
                                }
                                if (right_left == 0U) {
                                    free(best_path);
                                    return false;
                                }
                                ++col;
                                --right_left;
                            }

                            temporary[index++] = (Point){row, col};
                            sum += matrix[row * n + col];
                        }

                        double average = sum / (double)length;

                        if (average > best_average) {
                            best_average = average;
                            best_sum = sum;
                            best_length = length;
                            for (size_t i = 0U; i < length; ++i) {
                                best_path[i] = temporary[i];
                            }
                        }
                    }
                }
            }
        }
    }

    result->average = best_average;
    result->sum = best_sum;
    result->length = best_length;
    result->points = best_path;
    (void)cell_count;
    return true;
}

static void free_path_result(PathResult *result)
{
    if (result != NULL) {
        free(result->points);
        result->points = NULL;
        result->length = 0U;
        result->sum = 0.0;
        result->average = 0.0;
    }
}

int main(void)
{
    size_t n;
    size_t cell_count;
    size_t matrix_bytes;
    double *matrix = NULL;
    PathResult result = {0.0, 0.0, 0U, NULL};

    if (scanf("%zu", &n) != 1 || n == 0U ||
        !multiply_size(n, n, &cell_count) ||
        !multiply_size(cell_count, sizeof(*matrix), &matrix_bytes)) {
        fprintf(stderr, "Invalid matrix size\n");
        return EXIT_FAILURE;
    }

    matrix = malloc(matrix_bytes);
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < cell_count; ++i) {
        errno = 0;
        if (scanf("%lf", &matrix[i]) != 1 ||
            errno == ERANGE ||
            !isfinite(matrix[i])) {
            fprintf(stderr, "Invalid matrix value\n");
            free(matrix);
            return EXIT_FAILURE;
        }
    }

    if (!find_maximum_average_path(matrix, n, &result)) {
        fprintf(stderr, "Unable to find a path\n");
        free(matrix);
        return EXIT_FAILURE;
    }

    printf("%.10f\n", result.average);
    printf("%zu\n", result.length);

    for (size_t i = 0U; i < result.length; ++i) {
        printf("%zu %zu\n",
               result.points[i].row,
               result.points[i].col);
    }

    free_path_result(&result);
    free(matrix);
    return EXIT_SUCCESS;
}