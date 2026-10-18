#include <errno.h>
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

static int largest_inscribed_triangle(double semi_major, double semi_minor,
                                      Triangle *triangle)
{
    static const double angles[3] = {
        0.0,
        2.0943951023931954923,
        4.1887902047863909846
    };
    size_t i;

    if (triangle == NULL || !isfinite(semi_major) ||
        !isfinite(semi_minor) || semi_major <= 0.0 || semi_minor <= 0.0) {
        return 0;
    }

    for (i = 0U; i < 3U; ++i) {
        triangle->vertices[i].x = semi_major * cos(angles[i]);
        triangle->vertices[i].y = semi_minor * sin(angles[i]);

        if (!isfinite(triangle->vertices[i].x) ||
            !isfinite(triangle->vertices[i].y)) {
            return 0;
        }
    }

    triangle->area =
        (3.0 * sqrt(3.0) / 4.0) * semi_major * semi_minor;

    return isfinite(triangle->area) && triangle->area > 0.0;
}

static int parse_positive_double(const char *text, double *value)
{
    char *end = NULL;
    double parsed;

    if (text == NULL || value == NULL) {
        return 0;
    }

    errno = 0;
    parsed = strtod(text, &end);

    if (errno == ERANGE || end == text || *end != '\0' ||
        !isfinite(parsed) || parsed <= 0.0) {
        return 0;
    }

    *value = parsed;
    return 1;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    double semi_major;
    double semi_minor;
    Triangle triangle;
    size_t i;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <semi-major-axis> <semi-minor-axis>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    if (!parse_positive_double(argv[1], &semi_major) ||
        !parse_positive_double(argv[2], &semi_minor)) {
        fprintf(stderr, "Both semi-axis lengths must be finite positive numbers.\n");
        return EXIT_FAILURE;
    }

    if (!largest_inscribed_triangle(semi_major, semi_minor, &triangle)) {
        fprintf(stderr, "Unable to compute the largest inscribed triangle.\n");
        return EXIT_FAILURE;
    }

    printf("Maximum area: %.17g\n", triangle.area);

    for (i = 0U; i < 3U; ++i) {
        printf("Vertex %zu: (%.17g, %.17g)\n",
               i + 1U,
               triangle.vertices[i].x,
               triangle.vertices[i].y);
    }

    return EXIT_SUCCESS;
}