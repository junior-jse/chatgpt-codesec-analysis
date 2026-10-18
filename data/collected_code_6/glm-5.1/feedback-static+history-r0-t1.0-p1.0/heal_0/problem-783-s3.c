#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double h;
    double s;
    double v;
} Hsv;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Rgb;

Hsv rgb_to_hsv(Rgb rgb) {
    Hsv hsv;
    double r = rgb.r / 255.0;
    double g = rgb.g / 255.0;
    double b = rgb.b / 255.0;

    double max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    double min = r < g ? (r < b ? r : b) : (g < b ? g : b);
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
    } else if (max == r) {
        hsv.h = 60.0 * fmod(((g - b) / delta), 6.0);
    } else if (max == g) {
        hsv.h = 60.0 * (((b - r) / delta) + 2.0);
    } else {
        hsv.h = 60.0 * (((r - g) / delta) + 4.0);
    }

    if (hsv.h < 0.0) {
        hsv.h += 360.0;
    }

    return hsv;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <R> <G> <B>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    long val;

    val = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || val < 0 || val > 255) return EXIT_FAILURE;
    Rgb rgb;
    rgb.r = (unsigned char)val;

    val = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || val < 0 || val > 255) return EXIT_FAILURE;
    rgb.g = (unsigned char)val;

    val = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0' || val < 0 || val > 255) return EXIT_FAILURE;
    rgb.b = (unsigned char)val;

    Hsv hsv = rgb_to_hsv(rgb);

    printf("H: %f, S: %f, V: %f\n", hsv.h, hsv.s, hsv.v);

    return EXIT_SUCCESS;
}