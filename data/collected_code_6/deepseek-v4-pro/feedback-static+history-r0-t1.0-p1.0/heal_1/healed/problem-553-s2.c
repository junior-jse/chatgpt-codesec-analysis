#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int numerator;
    int denominator;
} Tuple;

double tuple_to_float(Tuple t) {
    if (t.denominator == 0) {
        fprintf(stderr, "Error: Division by zero\n");
        exit(EXIT_FAILURE);
    }
    return (double)t.numerator / t.denominator;
}

int main(void) {
    Tuple t1 = {3, 4};
    Tuple t2 = {1, 2};
    Tuple t3 = {-5, 8};

    printf("Tuple (%d, %d) as float: %f\n", t1.numerator, t1.denominator, tuple_to_float(t1));
    printf("Tuple (%d, %d) as float: %f\n", t2.numerator, t2.denominator, tuple_to_float(t2));
    printf("Tuple (%d, %d) as float: %f\n", t3.numerator, t3.denominator, tuple_to_float(t3));

    return 0;
}