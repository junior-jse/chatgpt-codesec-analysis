#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define PI 3.14159265358979323846
#define EPSILON 1e-9
#define MAX_ITERATIONS 10000

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[3];
    double area;
} Triangle;

double calculate_area(const Point *p1, const Point *p2, const Point *p3) {
    if (p1 == NULL || p2 == NULL || p3 == NULL) {
        return -1.0;
    }
    double area = 0.5 * fabs(p1->x * (p2->y - p3->y) + 
                             p2->x * (p3->y - p1->y) + 
                             p3->x * (p1->y - p2->y));
    return area;
}

int point_on_ellipse(const Point *p, double a, double b) {
    if (p == NULL || a <= 0.0 || b <= 0.0) {
        return 0;
    }
    double value = (p->x * p->x) / (a * a) + (p->y * p->y) / (b * b);
    return fabs(value - 1.0) < EPSILON;
}

void parametric_point(Point *p, double t, double a, double b) {
    if (p == NULL) {
        return;
    }
    p->x = a * cos(t);
    p->y = b * sin(t);
}

int find_largest_triangle(double a, double b, Triangle *result) {
    if (result == NULL || a <= 0.0 || b <= 0.0) {
        return -1;
    }
    
    double max_area = 0.0;
    double step = 2.0 * PI / MAX_ITERATIONS;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double t1 = i * step;
        Point p1;
        parametric_point(&p1, t1, a, b);
        
        for (int j = i + 1; j < MAX_ITERATIONS; j++) {
            double t2 = j * step;
            Point p2;
            parametric_point(&p2, t2, a, b);
            
            for (int k = j + 1; k < MAX_ITERATIONS; k++) {
                double t3 = k * step;
                Point p3;
                parametric_point(&p3, t3, a, b);
                
                double area = calculate_area(&p1, &p2, &p3);
                if (area < 0.0) {
                    continue;
                }
                
                if (area > max_area) {
                    max_area = area;
                    result->vertices[0] = p1;
                    result->vertices[1] = p2;
                    result->vertices[2] = p3;
                    result->area = area;
                }
            }
        }
    }
    
    if (max_area < EPSILON) {
        return -1;
    }
    
    return 0;
}

void print_triangle(const Triangle *t) {
    if (t == NULL) {
        return;
    }
    printf("Largest Triangle Area: %.6f\n", t->area);
    printf("Vertices:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Point %d: (%.6f, %.6f)\n", i + 1, 
               t->vertices[i].x, t->vertices[i].y);
    }
}

int main(void) {
    double a, b;
    Triangle largest;
    
    printf("Enter semi-major axis (a): ");
    if (scanf("%lf", &a) != 1) {
        fprintf(stderr, "Error: Invalid input for semi-major axis\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter semi-minor axis (b): ");
    if (scanf("%lf", &b) != 1) {
        fprintf(stderr, "Error: Invalid input for semi-minor axis\n");
        return EXIT_FAILURE;
    }
    
    if (a <= 0.0 || b <= 0.0) {
        fprintf(stderr, "Error: Axes must be positive values\n");
        return EXIT_FAILURE;
    }
    
    if (a > DBL_MAX / 2.0 || b > DBL_MAX / 2.0) {
        fprintf(stderr, "Error: Axes values too large\n");
        return EXIT_FAILURE;
    }
    
    int status = find_largest_triangle(a, b, &largest);
    if (status != 0) {
        fprintf(stderr, "Error: Failed to find largest triangle\n");
        return EXIT_FAILURE;
    }
    
    print_triangle(&largest);
    
    return EXIT_SUCCESS;
}