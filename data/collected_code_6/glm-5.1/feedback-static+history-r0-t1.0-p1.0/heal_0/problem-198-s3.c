#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare_doubles(const void *a, const void *b) {
    double diff = *(const double *)a - *(const double *)b;
    if (diff < 0.0) return -1;
    if (diff > 0.0) return 1;
    return 0;
}

void find_largest_inscribed_triangle(double a, double b, double *area, double vertices[3][2]) {
    if (a <= 0.0 || b <= 0.0) {
        *area = 0.0;
        vertices[0][0] = 0.0; vertices[0][1] = 0.0;
        vertices[1][0] = 0.0; vertices[1][1] = 0.0;
        vertices[2][0] = 0.0; vertices[2][1] = 0.0;
        return;
    }

    int n = 1000000;
    double step = 2.0 * M_PI / n;
    double *angles = (double *)malloc(n * sizeof(double));
    if (angles == NULL) {
        *area = 0.0;
        return;
    }

    for (int i = 0; i < n; i++) {
        double t = -M_PI + i * step;
        double alpha = atan2(a * sin(t), b * cos(t));
        if (alpha < 0.0) alpha += 2.0 * M_PI;
        angles[i] = alpha;
    }

    qsort(angles, n, sizeof(double), compare_doubles);

    double max_found = 0.0;
    double best_t0 = 0.0;
    double best_t1 = 0.0;
    double best_t2 = 0.0;

    for (int i = 0; i < n; i++) {
        double t0 = angles[i];
        double t1 = angles[(i + 1) % n];
        int j = (i + n / 3) % n;
        double t2 = angles[j];

        double x0 = a * cos(t0), y0 = b * sin(t0);
        double x1 = a * cos(t1), y1 = b * sin(t1);
        double x2 = a * cos(t2), y2 = b * sin(t2);

        double current_area = 0.5 * fabs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));

        if (current_area > max_found) {
            max_found = current_area;
            best_t0 = t0;
            best_t1 = t1;
            best_t2 = t2;
        }
    }

    free(angles);

    vertices[0][0] = a * cos(best_t0); vertices[0][1] = b * sin(best_t0);
    vertices[1][0] = a * cos(best_t1); vertices[1][1] = b * sin(best_t1);
    vertices[2][0] = a * cos(best_t2); vertices[2][1] = b * sin(best_t2);

    *area = max_found;
}

int main() {
    double a = 3.0;
    double b = 2.0;
    double area;
    double vertices[3][2];

    find_largest_inscribed_triangle(a, b, &area, vertices);

    printf("Ellipse semi-axes: a = %.4f, b = %.4f\n", a, b);
    printf("Largest Inscribed Triangle Area: %.4f\n", area);
    printf("Analytical Maximum Area (3*sqrt(3)*a*b/4): %.4f\n", 3.0 * sqrt(3.0) * a * b / 4.0);
    printf("Vertices:\n");
    for (int i = 0; i < 3; i++) {
        printf("  (%.4f, %.4f)\n", vertices[i][0], vertices[i][1]);
    }

    return 0;
}