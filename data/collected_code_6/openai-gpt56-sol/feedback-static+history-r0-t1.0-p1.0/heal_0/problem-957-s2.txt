#include <limits.h>
#include <stdio.h>

int rightmost_set_bit_position(unsigned int value, unsigned int *position)
{
    if (position == NULL || value == 0U) {
        return 0;
    }

    unsigned int index = 1U;

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
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (!rightmost_set_bit_position(value, &position)) {
        printf("No set bit\n");
        return 0;
    }

    printf("%u\n", position);
    return 0;
}