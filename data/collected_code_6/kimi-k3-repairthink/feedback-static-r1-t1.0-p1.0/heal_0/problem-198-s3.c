#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define NUM_SAMPLES 1000

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    double a;
    double b;
} Ellipse;

double calculate_triangle_area(Point p1, Point p2, Point p3) {
    return fabs((p1.x * (p2.y - p3.y) + 
                 p2.x * (p3.y - p1.y) + 
                 p3.x * (p1.y - p2.y)) / 2.0);
}

Point get_ellipse_point(Ellipse e, double t) {
    Point p;
    p.x = e.a * cos(t);
    p.y = e.b * sin(t);
    return p;
}

double find_largest_triangle(Ellipse e, Point *p1, Point *p2, Point *p3) {
    double max_area = 0.0;
    double t1, t2, t3;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        t1 = (2.0 * PI * i) / NUM_SAMPLES;
        for (int j = i + 1; j < NUM_SAMPLES; j++) {
            t2 = (2.0 * PI * j) / NUM_SAMPLES;
            for (int k = j + 1; k < NUM_SAMPLES; k++) {
                t3 = (2.0 * PI * k) / NUM_SAMPLES;
                
                Point pt1 = get_ellipse_point(e, t1);
                Point pt2 = get_ellipse_point(e, t2);
                Point pt3 = get_ellipse_point(e, t3);
                
                double area = calculate_triangle_area(pt1, pt2, pt3);
                
                if (area > max_area) {
                    max_area = area;
                    *p1 = pt1;
                    *p2 = pt2;
                    *p3 = pt3;
                }
            }
        }
    }
    
    return max_area;
}

double find_largest_triangle_analytical(Ellipse e, Point *p1, Point *p2, Point *p3) {
    double t1 = 0.0;
    double t2 = 2.0 * PI / 3.0;
    double t3 = 4.0 * PI / 3.0;
    
    *p1 = get_ellipse_point(e, t1);
    *p2 = get_ellipse_point(e, t2);
    *p3 = get_ellipse_point(e, t3);
    
    return (3.0 * sqrt(3.0) / 4.0) * e.a * e.b;
}

int main(void) {
    Ellipse e;
    Point p1, p2, p3;
    
    printf("Enter semi-major axis (a): ");
    if (scanf("%lf", &e.a) != 1 || e.a <= 0.0) {
        fprintf(stderr, "Invalid input for semi-major axis\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter semi-minor axis (b): ");
    if (scanf("%lf", &e.b) != 1 || e.b <= 0.0) {
        fprintf(stderr, "Invalid input for semi-minor axis\n");
        return EXIT_FAILURE;
    }
    
    double max_area = find_largest_triangle_analytical(e, &p1, &p2, &p3);
    
    printf("\nLargest inscribed triangle:\n");
    printf("Maximum area: %.6f\n", max_area);
    printf("Vertex 1: (%.6f, %.6f)\n", p1.x, p1.y);
    printf("Vertex 2: (%.6f, %.6f)\n", p2.x, p2.y);
    printf("Vertex 3: (%.6f, %.6f)\n", p3.x, p3.y);
    
    return EXIT_SUCCESS;
}