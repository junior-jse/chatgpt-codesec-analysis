#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool is_even(int number) {
    return (number % 2) == 0;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder srand: This function is not sufficiently random for security-related functions such as key and nonce creation (CWE-327). Use a more secure technique for acquiring random values. (risk 3, random)
     */
    srand(time(NULL));
    int num = rand();

    if (is_even(num)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}