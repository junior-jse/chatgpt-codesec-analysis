#include <stdio.h>
#include <stdbool.h>

bool is_even(int num) {
    return (num % 2) == 0;
}

int main(void) {
    int val = 4;
    if (is_even(val)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}