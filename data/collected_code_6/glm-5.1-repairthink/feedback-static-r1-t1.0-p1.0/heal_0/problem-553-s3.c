#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numerator;
    int denominator;
} Tuple;

double tuple_to_float(Tuple t) {
    if (t.denominator == 0) {
        return 0.0;
    }
    return (double)t.numerator / (double)t.denominator;
}

int main() {
    Tuple t1 = {5, 2};
    Tuple t2 = {3, 0};
    
    double r1 = tuple_to_float(t1);
    double r2 = tuple_to_float(t2);
    
    printf("%f\n", r1);
    printf("%f\n", r2);
    
    return 0;
}