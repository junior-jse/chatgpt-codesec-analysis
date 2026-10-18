#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double r;
    double g;
    double b;
} RGB;

typedef struct {
    double h;
    double s;
    double v;
} HSV;

static int rgb_to_hsv(const RGB *rgb, HSV *hsv)
{
    double max_value;
    double min_value;
    double delta;

    if (rgb == NULL || hsv == NULL) {
        return -1;
    }

    if (!isfinite(rgb->r) || !isfinite(rgb->g) || !isfinite(rgb->b) ||
        rgb->r < 0.0 || rgb->r > 255.0 ||
        rgb->g < 0.0 || rgb->g > 255.0 ||
        rgb->b < 0.0 || rgb->b > 255.0) {
        return -1;
    }

    max_value = fmax(rgb->r, fmax(rgb->g, rgb->b));
    min_value = fmin(rgb->r, fmin(rgb->g, rgb->b));
    delta = max_value - min_value;

    hsv->v = max_value / 255.0;

    if (max_value == 0.0) {
        hsv->s = 0.0;
        hsv->h = 0.0;
        return 0;
    }

    hsv->s = delta / max_value;

    if (delta == 0.0) {
        hsv->h = 0.0;
    } else if (max_value == rgb->r) {
        hsv->h = 60.0 * fmod((rgb->g - rgb->b) / delta, 6.0);
    } else if (max_value == rgb->g) {
        hsv->h = 60.0 * (((rgb->b - rgb->r) / delta) + 2.0);
    } else {
        hsv->h = 60.0 * (((rgb->r - rgb->g) / delta) + 4.0);
    }

    if (hsv->h < 0.0) {
        hsv->h += 360.0;
    }

    return 0;
}

int main(void)
{
    RGB rgb;
    HSV hsv;

    if (printf("Enter RGB values (0-255): ") < 0) {
        return EXIT_FAILURE;
    }

    if (scanf("%lf %lf %lf", &rgb.r, &rgb.g, &rgb.b) != 3) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (rgb_to_hsv(&rgb, &hsv) != 0) {
        fprintf(stderr, "RGB values must be finite and between 0 and 255\n");
        return EXIT_FAILURE;
    }

    if (printf("HSV: %.2f degrees, %.2f%%, %.2f%%\n",
               hsv.h, hsv.s * 100.0, hsv.v * 100.0) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}