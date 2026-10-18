#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct {
    double h;
    double s;
    double v;
} HSV;

static HSV rgb_to_hsv(RGB rgb)
{
    const double r = rgb.r / 255.0;
    const double g = rgb.g / 255.0;
    const double b = rgb.b / 255.0;
    const double max_value = fmax(r, fmax(g, b));
    const double min_value = fmin(r, fmin(g, b));
    const double delta = max_value - min_value;
    HSV hsv = {0.0, 0.0, max_value};

    if (max_value > 0.0) {
        hsv.s = delta / max_value;
    }

    if (delta > 0.0) {
        if (max_value == r) {
            hsv.h = 60.0 * fmod((g - b) / delta, 6.0);
        } else if (max_value == g) {
            hsv.h = 60.0 * (((b - r) / delta) + 2.0);
        } else {
            hsv.h = 60.0 * (((r - g) / delta) + 4.0);
        }

        if (hsv.h < 0.0) {
            hsv.h += 360.0;
        }
    }

    return hsv;
}

int main(void)
{
    unsigned int r;
    unsigned int g;
    unsigned int b;

    if (scanf("%u %u %u", &r, &g, &b) != 3) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (r > 255U || g > 255U || b > 255U) {
        fprintf(stderr, "RGB values must be between 0 and 255\n");
        return EXIT_FAILURE;
    }

    const RGB rgb = {
        (unsigned char)r,
        (unsigned char)g,
        (unsigned char)b
    };
    const HSV hsv = rgb_to_hsv(rgb);

    if (printf("H: %.2f\nS: %.4f\nV: %.4f\n", hsv.h, hsv.s, hsv.v) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}