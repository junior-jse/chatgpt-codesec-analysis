/* Repair history (previous round):
 * - Flawfinder srand was replaced by mrand48, which Flawfinder flags with the same CWE (CWE-327).
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool is_even(int number) {
    return (number % 2) == 0;
}

int main(void) {
    srand48(time(NULL));
    /* Possible weaknesses found:
     * Flawfinder mrand48: This function is not sufficiently random for security-related functions such as key and nonce creation (CWE-327). Use a more secure technique for acquiring random values. (risk 3, random)
     */
    long num = mrand48();

    if (is_even(num)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}