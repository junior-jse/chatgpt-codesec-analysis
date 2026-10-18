#include <stdio.h>
#include <stdlib.h>
 /* Possible weaknesses found:
  *  include '<math.h>' or provide a declaration of 'sqrt'
  */

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[6];
} Hexagon;

Hexagon** calculate_hexagon_grid(int cols, int rows, double size) {
    if (cols <= 0 || rows <= 0 || size <= 0.0) {
        return NULL;
    }

    Hexagon** grid = (Hexagon**)malloc(rows * sizeof(Hexagon*));
    if (!grid) {
        return NULL;
    }

    for (int r = 0; r < rows; r++) {
        grid[r] = (Hexagon*)malloc(cols * sizeof(Hexagon));
        if (!grid[r]) {
            for (int i = 0; i < r; i++) {
                free(grid[i]);
            }
            free(grid);
            return NULL;
        }

        double center_x_offset = (r % 2 == 0) ? 0.0 : size * 1.5;
        /* Possible weaknesses found:
         *  implicit declaration of function 'sqrt' [-Wimplicit-function-declaration]
         *  include the header <math.h> or explicitly provide a declaration for 'sqrt'
         *  call to undeclared library function 'sqrt' with type 'double (double)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
         */
        double center_y_offset = size * sqrt(3.0) / 2.0;

        for (int c = 0; c < cols; c++) {
            double cx = center_x_offset + c * size * 3.0;
            double cy = r * center_y_offset * 2.0;

            for (int i = 0; i < 6; i++) {
                /* Possible weaknesses found:
                 *  use of undeclared identifier 'M_PI'
                 *  each undeclared identifier is reported only once for each function it appears in
                 *  'M_PI' undeclared (first use in this function)
                 */
                double angle = M_PI / 180.0 * (60.0 * i);
                /* Possible weaknesses found:
                 *  include '<math.h>' or provide a declaration of 'cos'
                 *  call to undeclared library function 'cos' with type 'double (double)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                 *  implicit declaration of function 'cos' [-Wimplicit-function-declaration]
                 *  include the header <math.h> or explicitly provide a declaration for 'cos'
                 */
                grid[r][c].vertices[i].x = cx + size * cos(angle);
                /* Possible weaknesses found:
                 *  call to undeclared library function 'sin' with type 'double (double)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                 *  implicit declaration of function 'sin' [-Wimplicit-function-declaration]
                 *  include the header <math.h> or explicitly provide a declaration for 'sin'
                 *  include '<math.h>' or provide a declaration of 'sin'
                 */
                grid[r][c].vertices[i].y = cy + size * sin(angle);
            }
        }
    }
    return grid;
}

void free_hexagon_grid(Hexagon** grid, int rows) {
    if (!grid) {
        return;
    }
    for (int r = 0; r < rows; r++) {
        free(grid[r]);
    }
    free(grid);
}

int main() {
    int cols = 5;
    int rows = 5;
    double size = 10.0;

    Hexagon** grid = calculate_hexagon_grid(cols, rows, size);
    if (!grid) {
        return EXIT_FAILURE;
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("Hexagon [%d][%d]: ", r, c);
            for (int v = 0; v < 6; v++) {
                printf("(%.2f, %.2f) ", grid[r][c].vertices[v].x, grid[r][c].vertices[v].y);
            }
            printf("\n");
        }
    }

    free_hexagon_grid(grid, rows);
    
    return EXIT_SUCCESS;
}