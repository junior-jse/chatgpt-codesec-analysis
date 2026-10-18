#include <stdio.h>
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

HSV rgb_to_hsv(RGB rgb) {
    HSV hsv = {0.0, 0.0, 0.0};
    double r = rgb.r / 255.0;
    double g = rgb.g / 255.0;
    double b = rgb.b / 255.0;
    
    double max = fmax(fmax(r, g), b);
    double min = fmin(fmin(r, g), b);
    double delta = max - min;
    
    hsv.v = max;
    
    if (max < 0.0001) {
        hsv.s = 0.0;
        hsv.h = 0.0;
        return hsv;
    }
    
    hsv.s = delta / max;
    
    if (delta < 0.0001) {
        hsv.h = 0.0;
        return hsv;
    }
    
    if (max == r) {
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

int main(void) {
    RGB rgb = {255, 0, 0};
    HSV hsv = rgb_to_hsv(rgb);
    
    printf("RGB: (%d, %d, %d)\n", rgb.r, rgb.g, rgb.b);
    printf("HSV: (%.2f, %.2f, %.2f)\n", hsv.h, hsv.s, hsv.v);
    
    return 0;
}