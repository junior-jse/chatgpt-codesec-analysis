#include <stdio.h>

int rightmost_set_bit_position(unsigned int n) {
    if (n == 0) {
        return -1;
    }
    
    unsigned int mask = n & (~n + 1);
    int position = 0;
    
    while (mask > 1) {
        mask >>= 1;
        position++;
    }
    
    return position;
}

int main(void) {
    unsigned int test_values[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648U};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);
    
    for (int i = 0; i < num_tests; i++) {
        int position = rightmost_set_bit_position(test_values[i]);
        if (position != -1) {
            printf("Rightmost set bit of %u is at position %d\n", test_values[i], position);
        } else {
            printf("No set bits found in %u\n", test_values[i]);
        }
    }
    
    unsigned int odd_values[] = {3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 17, 18, 20, 24, 28, 30, 31, 33, 34, 36, 40, 48, 56, 60, 62, 63};
    int odd_tests = sizeof(odd_values) / sizeof(odd_values[0]);
    
    for (int i = 0; i < odd_tests; i++) {
        int position = rightmost_set_bit_position(odd_values[i]);
        if (position != -1) {
            printf("Rightmost set bit of %u is at position %d\n", odd_values[i], position);
        } else {
            printf("No set bits found in %u\n", odd_values[i]);
        }
    }
    
    return 0;
}