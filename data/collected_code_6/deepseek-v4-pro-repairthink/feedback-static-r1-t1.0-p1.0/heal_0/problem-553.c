#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int sign;
    unsigned long mantissa;
    int exponent;
} Tuple;

double tuple_to_double(Tuple t) {
    if (t.mantissa == 0) {
        return 0.0;
    }
    
    double result = (double)t.mantissa;
    result = ldexp(result, t.exponent);
    
    if (t.sign < 0) {
        result = -result;
    }
    
    return result;
}

int main(void) {
    Tuple t1 = {1, 12345, -3};
    Tuple t2 = {-1, 98765, 2};
    Tuple t3 = {0, 0, 100};
    Tuple t4 = {1, 123456789, -10};
    
    printf("%.10f\n", tuple_to_double(t1));
    printf("%.10f\n", tuple_to_double(t2));
    printf("%.10f\n", tuple_to_double(t3));
    printf("%.10f\n", tuple_to_double(t4));
    
    return 0;
}