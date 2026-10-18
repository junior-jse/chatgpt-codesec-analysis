#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>

typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    Point *points;
    int length;
    int capacity;
} Path;

int init_path(Path *path, int capacity) {
    if (path == NULL || capacity <= 0) {
        return 0;
    }
    path->points = (Point *)malloc((size_t)capacity * sizeof(Point));
    if (path->points == NULL) {
        return 0;
    }
    path->length = 0;
    path->capacity = capacity;
    return 1;
}

void free_path(Path *path) {
    if (path != NULL && path->points != NULL) {
        free(path->points);
        path->points = NULL;
        path->length = 0;
        path->capacity = 0;
    }
}

int copy_path(Path *dest, const Path *src) {
    if (dest == NULL || src == NULL || dest->capacity < src->length) {
        return 0;
    }
    int i;
    for (i = 0; i < src->length; i++) {
        dest->points[i] = src->points[i];
    }
    dest->length = src->length;
    return 1;
}

int add_point(Path *path, int row, int col) {
    if (path == NULL || path->length >= path->capacity) {
        return 0;
    }
    path->points[path->length].row = row;
    path->points[path->length].col = col;
    path->length++;
    return 1;
}

void remove_last_point(Path *path) {
    if (path != NULL && path->length > 0) {
        path->length--;
    }
}

void find_max_average_path_util(int **matrix, int n, int row, int col,
                                int **visited, Path *current_path,
                                double current_sum, Path *max_path,
                                double *max_average) {
    if (matrix == NULL || visited == NULL || current_path == NULL ||
        max_path == NULL || max_average == NULL || n <= 0) {
        return;
    }

    if (row < 0 || row >= n || col < 0 || col >= n || visited[row][col]) {
        return;
    }

    if (!add_point(current_path, row, col)) {
        return;
    }

    visited[row][col] = 1;
    current_sum += (double)matrix[row][col];

    if (row == n - 1 && col == n - 1) {
        double average = current_sum / (double)current_path->length;
        if (average > *max_average) {
            *max_average = average;
            copy_path(max_path, current_path);
        }
    } else {
        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int i;
        for (i = 0; i < 4; i++) {
            int new_row = row + directions[i][0];
            int new_col = col + directions[i][1];
            find_max_average_path_util(matrix, n, new_row, new_col, visited,
                                       current_path, current_sum, max_path,
                                       max_average);
        }
    }

    visited[row][col] = 0;
    remove_last_point(current_path);
}

double find_max_average_path(int **matrix, int n, Path *result_path) {
    if (matrix == NULL || result_path == NULL || n <= 0) {
        return -DBL_MAX;
    }

    int **visited = (int **)malloc((size_t)n * sizeof(int *));
    if (visited == NULL) {
        return -DBL_MAX;
    }

    int i;
    for (i = 0; i < n; i++) {
        visited[i] = (int *)calloc((size_t)n, sizeof(int));
        if (visited[i] == NULL) {
            int j;
            for (j = 0; j < i; j++) {
                free(visited[j]);
            }
            free(visited);
            return -DBL_MAX;
        }
    }

    Path current_path;
    if (!init_path(&current_path, n * n)) {
        for (i = 0; i < n; i++) {
            free(visited[i]);
        }
        free(visited);
        return -DBL_MAX;
    }

    double max_average = -DBL_MAX;

    find_max_average_path_util(matrix, n, 0, 0, visited, &current_path,
                               0.0, result_path, &max_average);

    free_path(&current_path);
    for (i = 0; i < n; i++) {
        free(visited[i]);
    }
    free(visited);

    return max_average;
}

int main(void) {
    int n;
    
    printf("Enter matrix size (n): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 10) {
        fprintf(stderr, "Invalid matrix size\n");
        return EXIT_FAILURE;
    }

    int **matrix = (int **)malloc((size_t)n * sizeof(int *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    int i, j;
    for (i = 0; i < n; i++) {
        matrix[i] = (int *)malloc((size_t)n * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return EXIT_FAILURE;
        }
    }

    printf("Enter matrix elements:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                for (i = 0; i < n; i++) {
                    free(matrix[i]);
                }
                free(matrix);
                return EXIT_FAILURE;
            }
        }
    }

    Path result_path;
    if (!init_path(&result_path, n * n)) {
        fprintf(stderr, "Memory allocation failed\n");
        for (i = 0; i < n; i++) {
            free(matrix[i]);
        }
        free(matrix);
        return EXIT_FAILURE;
    }

    double max_average = find_max_average_path(matrix, n, &result_path);

    if (max_average == -DBL_MAX) {
        fprintf(stderr, "No path found\n");
    } else {
        printf("Maximum average: %.2f\n", max_average);
        printf("Path: ");
        for (i = 0; i < result_path.length; i++) {
            printf("(%d,%d)", result_path.points[i].row, result_path.points[i].col);
            if (i < result_path.length - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
    }

    free_path(&result_path);
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return EXIT_SUCCESS;
}