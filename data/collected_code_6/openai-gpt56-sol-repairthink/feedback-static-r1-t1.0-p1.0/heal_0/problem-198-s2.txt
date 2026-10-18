#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point vertices[3];
    double area;
} Triangle;

static int largest_inscribed_triangle(double semi_major,
                                      double semi_minor,
                                      double rotation,
                                      Triangle *triangle)
{
    const double two_pi_over_three = 2.0 * acos(-1.0) / 3.0;
    const double cosine_rotation = cos(rotation);
    const double sine_rotation = sin(rotation);
    size_t i;

    if (triangle == NULL ||
        !isfinite(semi_major) ||
        !isfinite(semi_minor) ||
        !isfinite(rotation) ||
        semi_major <= 0.0 ||
        semi_minor <= 0.0) {
        return 0;
    }

    triangle->area = 3.0 * sqrt(3.0) * semi_major * semi_minor / 4.0;

    if (!isfinite(triangle->area)) {
        return 0;
    }

    for (i = 0U; i < 3U; ++i) {
        const double angle = (double)i * two_pi_over_three;
        const double x = semi_major * cos(angle);
        const double y = semi_minor * sin(angle);

        triangle->vertices[i].x =
            x * cosine_rotation - y * sine_rotation;
        triangle->vertices[i].y =
            x * sine_rotation + y * cosine_rotation;

        if (!isfinite(triangle->vertices[i].x) ||
            !isfinite(triangle->vertices[i].y)) {
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    double semi_major;
    double semi_minor;
    double rotation_degrees;
    double rotation_radians;
    Triangle triangle;
    size_t i;

    if (printf("Enter semi-major axis, semi-minor axis, and rotation in degrees: ") < 0) {
        return EXIT_FAILURE;
    }

    if (scanf("%lf %lf %lf",
              &semi_major,
              &semi_minor,
              &rotation_degrees) != 3) {
        fprintf(stderr, "Invalid input.\n");
        return EXIT_FAILURE;
    }

    rotation_radians = rotation_degrees * acos(-1.0) / 180.0;

    if (!largest_inscribed_triangle(semi_major,
                                    semi_minor,
                                    rotation_radians,
                                    &triangle)) {
        fprintf(stderr, "Unable to compute the triangle.\n");
        return EXIT_FAILURE;
    }

    if (printf("Maximum area: %.10g\n", triangle.area) < 0) {
        return EXIT_FAILURE;
    }

    for (i = 0U; i < 3U; ++i) {
        if (printf("Vertex %zu: (%.10g, %.10g)\n",
                   i + 1U,
                   triangle.vertices[i].x,
                   triangle.vertices[i].y) < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}