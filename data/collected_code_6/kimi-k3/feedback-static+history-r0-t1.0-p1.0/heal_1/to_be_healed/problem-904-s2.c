#include <stdio.h>
#include <stdbool.h>

bool is_even(int number)
{
    return number % 2 == 0;
}

int main(void)
{
    /* Possible weaknesses found:
     *  Assignment 'number=4', assigned value is 4
     */
    int number = 4;

    /* Possible weaknesses found:
     *  Calling function 'is_even' returns 1
     *  Condition 'is_even(number)' is always true [knownConditionTrueFalse]
     *  Condition 'is_even(number)' is always true
     */
    if (is_even(number)) {
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }

    return 0;
}