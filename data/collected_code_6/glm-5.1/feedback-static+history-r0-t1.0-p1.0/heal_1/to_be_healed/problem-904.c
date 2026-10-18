#include <stdio.h>
#include <stdbool.h>

bool is_even(int number) {
    return (number % 2) == 0;
}

int main(void) {
    /* Possible weaknesses found:
     *  Assignment 'num=4', assigned value is 4
     */
    int num = 4;
    /* Possible weaknesses found:
     *  Condition 'is_even(num)' is always true [knownConditionTrueFalse]
     *  Calling function 'is_even' returns 1
     *  Condition 'is_even(num)' is always true
     */
    if (is_even(num)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}