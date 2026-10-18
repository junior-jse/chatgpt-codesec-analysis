#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool is_even(int number) {
    return (number % 2) == 0;
}

int main(void) {
    srand48(time(NULL));
    long num = mrand48();

    if (is_even(num)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}