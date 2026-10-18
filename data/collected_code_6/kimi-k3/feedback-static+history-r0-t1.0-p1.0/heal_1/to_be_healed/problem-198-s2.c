#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[3];
    double area;
} Triangle;

double calculate_area(Point p1, Point p2, Point p3) {
    return 0.5 * fabs(p1.x * (p2.y - p3.y) + 
                      p2.x * (p3.y - p1.y) + 
                      p3.x * (p1.y - p2.y));
}

Point get_ellipse_point(double a, double b, double theta) {
    Point p;
    p.x = a * cos(theta);
    p.y = b * sin(theta);
    return p;
}

int find_largest_triangle(double a, double b, Triangle *result) {
    if (result == NULL || a <= 0.0 || b <= 0.0) {
        return -1;
    }
    
    double max_area = 0.0;
    /* Possible weaknesses found:
     *  Variable 'optimal_angles' can be declared as const array [constVariable]
     */
    double optimal_angles[3] = {0.0, 2.0 * M_PI / 3.0, 4.0 * M_PI / 3.0};
    
    for (int i = 0; i < 3; i++) {
        result->vertices[i] = get_ellipse_point(a, b, optimal_angles[i]);
    }
    
    max_area = (3.0 * sqrt(3.0) / 4.0) * a * b;
    result->area = max_area;
    
    return 0;
}

int main(void) {
    double a = 5.0;
    double b = 3.0;
    Triangle largest;
    
    if (find_largest_triangle(a, b, &largest) != 0) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return EXIT_FAILURE;
    }
    
    printf("Largest triangle inscribed in ellipse (a=%.2f, b=%.2f):\n", a, b);
    printf("Maximum area: %.6f\n", largest.area);
    printf("Vertices:\n");
    
    for (int i = 0; i < 3; i++) {
        printf("  Vertex %d: (%.6f, %.6f)\n", 
               i + 1, largest.vertices[i].x, largest.vertices[i].y);
    }
    
    double calculated_area = calculate_area(largest.vertices[0], 
                                           largest.vertices[1], 
                                           largest.vertices[2]);
    printf("Verification area: %.6f\n", calculated_area);
    
    return EXIT_SUCCESS;
}