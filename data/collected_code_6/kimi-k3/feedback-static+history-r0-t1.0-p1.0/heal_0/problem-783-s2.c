#include <stdio.h>

typedef struct {
    float h;
    float s;
    float v;
} HSV;

HSV rgb_to_hsv(float r, float g, float b) {
    HSV hsv = {0.0f, 0.0f, 0.0f};
    float max, min, delta;
    
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;
    
    max = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
    min = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
    delta = max - min;
    
    hsv.v = max;
    
    if (max != 0.0f) {
        hsv.s = delta / max;
    } else {
        hsv.s = 0.0f;
        hsv.h = 0.0f;
        return hsv;
    }
    
    if (delta == 0.0f) {
        hsv.h = 0.0f;
    } else if (max == r) {
        hsv.h = 60.0f * (((g - b) / delta));
        if (hsv.h < 0.0f) {
            hsv.h += 360.0f;
        }
    } else if (max == g) {
        hsv.h = 60.0f * (((b - r) / delta) + 2.0f);
    } else {
        hsv.h = 60.0f * (((r - g) / delta) + 4.0f);
    }
    
    return hsv;
}

int main(void) {
    float r, g, b;
    HSV hsv;
    
    printf("Enter RGB values (0-255): ");
    if (scanf("%f %f %f", &r, &g, &b) != 3) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (r < 0.0f || r > 255.0f || g < 0.0f || g > 255.0f || b < 0.0f || b > 255.0f) {
        fprintf(stderr, "RGB values must be between 0 and 255\n");
        return 1;
    }
    
    hsv = rgb_to_hsv(r, g, b);
    
    printf("HSV: %.2f, %.2f%%, %.2f%%\n", hsv.h, hsv.s * 100.0f, hsv.v * 100.0f);
    
    return 0;
}