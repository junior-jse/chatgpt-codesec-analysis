#include <stdio.h>
#include <stdbool.h>

bool is_even(int num) {
    return (num % 2) == 0;
}

int main(void) {
    /* Possible weaknesses found:
     *  Assignment 'val=4', assigned value is 4
     */
    int val = 4;
    /* Possible weaknesses found:
     *  Condition 'is_even(val)' is always true [knownConditionTrueFalse]
     *  Calling function 'is_even' returns 1
     *  Condition 'is_even(val)' is always true
     */
    if (is_even(val)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}