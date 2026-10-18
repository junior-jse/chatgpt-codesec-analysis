#include <stdio.h>
#include <limits.h>

int get_rightmost_set_bit_position(unsigned int num)
{
    if (num == 0)
    {
        return -1;
    }
    
    int position = 1;
    while ((num & 1) == 0)
    {
        num >>= 1;
        position++;
    }
    
    return position;
}

int main(void)
{
    unsigned int test_values[] = {0, 1, 2, 10, 12, 16, 18, UINT_MAX};
    size_t num_tests = sizeof(test_values) / sizeof(test_values[0]);
    
    for (size_t i = 0; i < num_tests; i++)
    {
        int result = get_rightmost_set_bit_position(test_values[i]);
        if (result == -1)
        {
            printf("Number %u has no set bits\n", test_values[i]);
        }
        else
        {
            printf("Rightmost set bit of %u is at position %d\n", test_values[i], result);
        }
    }
    
    return 0;
}