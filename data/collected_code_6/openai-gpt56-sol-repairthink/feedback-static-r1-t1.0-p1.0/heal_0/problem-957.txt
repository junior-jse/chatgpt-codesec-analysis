#include <limits.h>
#include <stdio.h>

int rightmost_set_bit_position(unsigned int value)
{
    int position = 1;

    if (value == 0U) {
        return 0;
    }

    while ((value & 1U) == 0U) {
        value >>= 1U;
        ++position;
    }

    return position;
}

int main(void)
{
    unsigned int value;

    if (scanf("%u", &value) != 1) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    printf("%d\n", rightmost_set_bit_position(value));
    return 0;
}