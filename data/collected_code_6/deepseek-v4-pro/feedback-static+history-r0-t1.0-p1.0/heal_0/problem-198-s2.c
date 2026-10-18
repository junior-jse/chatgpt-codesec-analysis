#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point a;
    Point b;
    Point c;
    double area;
} Triangle;

static double distance(const Point *p1, const Point *p2) {
    double dx = p1->x - p2->x;
    double dy = p1->y - p2->y;
    return sqrt(dx * dx + dy * dy);
}

static double heron_area(const Point *a, const Point *b, const Point *c) {
    double ab = distance(a, b);
    double bc = distance(b, c);
    double ca = distance(c, a);
    double s = (ab + bc + ca) / 2.0;
    if (s <= ab || s <= bc || s <= ca) {
        return 0.0;
    }
    return sqrt(s * (s - ab) * (s - bc) * (s - ca));
}

static Point ellipse_point(double rx, double ry, double angle) {
    Point p;
    p.x = rx * cos(angle);
    p.y = ry * sin(angle);
    return p;
}

static double signed_area(const Point *a, const Point *b, const Point *c) {
    return 0.5 * ((b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x));
}

static int is_valid_triangle(const Point *a, const Point *b, const Point *c) {
    double area = fabs(signed_area(a, b, c));
    return area > 1e-10;
}

static Triangle find_largest_triangle(double rx, double ry) {
    Triangle best = {0};
    best.area = -1.0;
    
    const int steps = 180;
    const double step = (2.0 * M_PI) / steps;
    
    Point *points = (Point *)malloc(steps * sizeof(Point));
    if (points == NULL) {
        return best;
    }
    
    for (int i = 0; i < steps; i++) {
        points[i] = ellipse_point(rx, ry, i * step);
    }
    
    for (int i = 0; i < steps; i++) {
        for (int j = i + 1; j < steps; j++) {
            for (int k = j + 1; k < steps; k++) {
                if (!is_valid_triangle(&points[i], &points[j], &points[k])) {
                    continue;
                }
                double area = fabs(signed_area(&points[i], &points[j], &points[k]));
                if (area > best.area) {
                    best.area = area;
                    best.a = points[i];
                    best.b = points[j];
                    best.c = points[k];
                }
            }
        }
    }
    
    free(points);
    return best;
}

int main(void) {
    double rx = 5.0;
    double ry = 3.0;
    
    Triangle largest = find_largest_triangle(rx, ry);
    
    if (largest.area < 0) {
        fprintf(stderr, "Error: Failed to find triangle\n");
        return 1;
    }
    
    printf("Ellipse: rx=%.2f, ry=%.2f\n", rx, ry);
    printf("Largest inscribed triangle area: %.6f\n", largest.area);
    printf("Vertex 1: (%.6f, %.6f)\n", largest.a.x, largest.a.y);
    printf("Vertex 2: (%.6f, %.6f)\n", largest.b.x, largest.b.y);
    printf("Vertex 3: (%.6f, %.6f)\n", largest.c.x, largest.c.y);
    
    double check_area = fabs(signed_area(&largest.a, &largest.b, &largest.c));
    printf("Verification area: %.6f\n", check_area);
    
    return 0;
}