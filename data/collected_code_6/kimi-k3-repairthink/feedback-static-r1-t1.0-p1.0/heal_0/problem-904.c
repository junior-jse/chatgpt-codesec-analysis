#include <stdio.h>
#include <stdbool.h>

bool is_even(int number) {
    return number % 2 == 0;
}

int main(void) {
    int number;
    bool result;
    
    printf("Enter a number: ");
    
    if (scanf("%d", &number) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    result = is_even(number);
    
    if (result) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    
    return 0;
}