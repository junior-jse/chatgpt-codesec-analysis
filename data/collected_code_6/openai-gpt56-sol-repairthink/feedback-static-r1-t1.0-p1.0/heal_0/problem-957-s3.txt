#include <limits.h>
#include <stdio.h>

int rightmost_set_bit_position(unsigned int value, unsigned int *position)
{
    unsigned int index = 1U;

    if (position == NULL || value == 0U) {
        return 0;
    }

    while ((value & 1U) == 0U) {
        value >>= 1U;
        ++index;
    }

    *position = index;
    return 1;
}

int main(void)
{
    unsigned int value;
    unsigned int position;

    if (scanf("%u", &value) != 1) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    if (!rightmost_set_bit_position(value, &position)) {
        puts("No set bit");
        return 0;
    }

    printf("%u\n", position);
    return 0;
}