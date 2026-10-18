#include <stdio.h>
#include <stdlib.h>

int find_nth_digit(int numerator, int denominator, int n) {
    /* Possible weaknesses found:
     *  Assuming that condition 'denominator<=0' is not redundant
     */
    if (denominator <= 0 || n <= 0) {
        return -1;
    }
    long long num = (long long)numerator;
    /* Possible weaknesses found:
     *  Assignment 'den=(long long)denominator', assigned value is greater than 0
     */
    long long den = (long long)denominator;
    if (num < 0) {
        num = -num;
    }
    /* Possible weaknesses found:
     *  Condition 'den<0' is always false [knownConditionTrueFalse]
     *  Condition 'den<0' is always false
     */
    if (den < 0) {
        den = -den;
    }
    long long remainder = num % den;
    for (int i = 0; i < n; ++i) {
        remainder *= 10;
        /* Possible weaknesses found:
         *  Condition 'den==0' is always false
         *  Condition 'den==0' is always false [knownConditionTrueFalse]
         */
        if (den == 0) {
            return -1;
        }
        int digit = (int)(remainder / den);
        remainder %= den;
        if (i == n - 1) {
            return digit;
        }
    }
    return -1;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <numerator> <denominator> <n>\n", argv[0]);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     * Flawfinder atoi: Unless checked, the resulting number can exceed the expected range (CWE-190). If source untrusted, check both minimum and maximum, even if the input had no minus sign (large numbers can roll over into negative number; consider saving to an unsigned value if that is intended). (risk 2, integer)
     */
    int numerator = atoi(argv[1]);
    /* Possible weaknesses found:
     * Flawfinder atoi: Unless checked, the resulting number can exceed the expected range (CWE-190). If source untrusted, check both minimum and maximum, even if the input had no minus sign (large numbers can roll over into negative number; consider saving to an unsigned value if that is intended). (risk 2, integer)
     */
    int denominator = atoi(argv[2]);
    /* Possible weaknesses found:
     * Flawfinder atoi: Unless checked, the resulting number can exceed the expected range (CWE-190). If source untrusted, check both minimum and maximum, even if the input had no minus sign (large numbers can roll over into negative number; consider saving to an unsigned value if that is intended). (risk 2, integer)
     */
    int n = atoi(argv[3]);
    int result = find_nth_digit(numerator, denominator, n);
    if (result == -1) {
        return EXIT_FAILURE;
    }
    printf("%d\n", result);
    return EXIT_SUCCESS;
}