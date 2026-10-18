#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double area(double a, double b, double t1, double t2, double t3) {
    double x1 = a * cos(t1), y1 = b * sin(t1);
    double x2 = a * cos(t2), y2 = b * sin(t2);
    double x3 = a * cos(t3), y3 = b * sin(t3);
    double area_val = 0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    return area_val < 0 ? -area_val : area_val;
}

double find_max_area(double a, double b, double step) {
    if (a <= 0.0 || b <= 0.0 || step <= 0.0) {
        return 0.0;
    }
    double max_area = 0.0;
    double t1, t2, t3;
    for (t1 = 0.0; t1 < 2.0 * M_PI; t1 += step) {
        for (t2 = t1 + step; t2 < 2.0 * M_PI + t1; t2 += step) {
            for (t3 = t2 + step; t3 < (2.0 * M_PI + t2) - step; t3 += step) {
                double current_area = area(a, b, t1, t2, t3);
                if (current_area > max_area) {
                    max_area = current_area;
                }
            }
        }
    }
    return max_area;
}

int main(int argc, char * const argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <a> <b>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *endptr1, *endptr2;
    double a = strtod(argv[1], &endptr1);
    double b = strtod(argv[2], &endptr2);
    if (*endptr1 != '\0' || *endptr2 != '\0' || a <= 0.0 || b <= 0.0) {
        fprintf(stderr, "Error: a and b must be positive numbers\n");
        return EXIT_FAILURE;
    }
    double step = 0.001;
    double result = find_max_area(a, b, step);
    double theoretical = 1.5 * a * b * sqrt(3.0);
    printf("Calculated Maximum Triangle Area: %f\n", result);
    printf("Theoretical Maximum Triangle Area: %f\n", theoretical);
    return EXIT_SUCCESS;
}