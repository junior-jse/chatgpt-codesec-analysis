#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Rgb;

typedef struct {
    double h;
    double s;
    double v;
} Hsv;

Hsv rgb_to_hsv(Rgb rgb) {
    Hsv hsv;
    double r = rgb.r / 255.0;
    double g = rgb.g / 255.0;
    double b = rgb.b / 255.0;

    double max = r;
    if (g > max) max = g;
    if (b > max) max = b;

    double min = r;
    if (g < min) min = g;
    if (b < min) min = b;

    double delta = max - min;

    hsv.v = max;

    if (max == 0.0) {
        hsv.s = 0.0;
        hsv.h = 0.0;
        return hsv;
    }

    hsv.s = delta / max;

    if (delta == 0.0) {
        hsv.h = 0.0;
        return hsv;
    }

    if (max == r) {
        hsv.h = (g - b) / delta;
    } else if (max == g) {
        hsv.h = 2.0 + (b - r) / delta;
    } else {
        hsv.h = 4.0 + (r - g) / delta;
    }

    hsv.h *= 60.0;
    if (hsv.h < 0.0) {
        hsv.h += 360.0;
    }

    return hsv;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <r> <g> <b>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    long val_r = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || val_r < 0 || val_r > 255) {
        fprintf(stderr, "Invalid R value. Must be 0-255.\n");
        return EXIT_FAILURE;
    }

    long val_g = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || val_g < 0 || val_g > 255) {
        fprintf(stderr, "Invalid G value. Must be 0-255.\n");
        return EXIT_FAILURE;
    }

    long val_b = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0' || val_b < 0 || val_b > 255) {
        fprintf(stderr, "Invalid B value. Must be 0-255.\n");
        return EXIT_FAILURE;
    }

    Rgb rgb = {(unsigned char)val_r, (unsigned char)val_g, (unsigned char)val_b};
    Hsv hsv = rgb_to_hsv(rgb);

    printf("%f %f %f\n", hsv.h, hsv.s, hsv.v);

    return EXIT_SUCCESS;
}