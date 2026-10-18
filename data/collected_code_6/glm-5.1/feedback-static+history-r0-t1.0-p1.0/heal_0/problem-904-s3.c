#include <stdio.h>
#include <stdbool.h>

bool is_even(int number) {
    return (number & 1) == 0;
}

int main(int argc, char *argv[]) {
    int val = 4;
    bool result = is_even(val);
    printf("%d\n", result);
    return 0;
}