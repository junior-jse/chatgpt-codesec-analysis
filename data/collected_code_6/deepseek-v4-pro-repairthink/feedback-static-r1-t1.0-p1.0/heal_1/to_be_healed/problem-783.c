#include <stdio.h>
 /* Possible weaknesses found:
  *  include '<math.h>' or provide a declaration of 'fmodf'
  */

typedef struct {
    float r;
    float g;
    float b;
} RGB;

typedef struct {
    float h;
    float s;
    float v;
} HSV;

HSV rgb_to_hsv(RGB rgb) {
    HSV hsv;
    float max = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    float min = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    float delta = max - min;

    hsv.v = max;

    if (max == 0.0f) {
        hsv.s = 0.0f;
        hsv.h = 0.0f;
    } else {
        hsv.s = delta / max;

        if (delta == 0.0f) {
            hsv.h = 0.0f;
        } else {
            if (max == rgb.r) {
                /* Possible weaknesses found:
                 *  call to undeclared library function 'fmodf' with type 'float (float, float)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                 *  include the header <math.h> or explicitly provide a declaration for 'fmodf'
                 *  implicit declaration of function 'fmodf' [-Wimplicit-function-declaration]
                 */
                hsv.h = 60.0f * fmodf((rgb.g - rgb.b) / delta, 6.0f);
            } else if (max == rgb.g) {
                hsv.h = 60.0f * ((rgb.b - rgb.r) / delta + 2.0f);
            } else {
                hsv.h = 60.0f * ((rgb.r - rgb.g) / delta + 4.0f);
            }

            if (hsv.h < 0.0f) {
                hsv.h += 360.0f;
            }
        }
    }

    return hsv;
}

int main(void) {
    RGB input = {0.5f, 0.2f, 0.8f};
    HSV output = rgb_to_hsv(input);

    printf("R: %.2f, G: %.2f, B: %.2f\n", input.r, input.g, input.b);
    printf("H: %.2f, S: %.2f, V: %.2f\n", output.h, output.s, output.v);

    return 0;
}