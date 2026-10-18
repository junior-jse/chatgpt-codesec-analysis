#include <stdio.h>
#include <limits.h>

int getRightmostSetBitPosition(unsigned int n) {
    if (n == 0) {
        return -1;
    }
    
    int position = 1;
    while ((n & 1) == 0) {
        n >>= 1;
        position++;
    }
    
    return position;
}

int main(void) {
    unsigned int testValues[] = {0, 1, 2, 4, 8, 12, 18, 24, 32, 40};
    size_t numTests = sizeof(testValues) / sizeof(testValues[0]);
    
    for (size_t i = 0; i < numTests; i++) {
        int result = getRightmostSetBitPosition(testValues[i]);
        if (result == -1) {
            printf("Number %u has no set bits\n", testValues[i]);
        } else {
            printf("Rightmost set bit of %u is at position %d\n", testValues[i], result);
        }
    }
    
    return 0;
}