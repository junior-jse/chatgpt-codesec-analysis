#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool is_even(int number)
{
    return number % 2 == 0;
}

int main(void)
{
    int number;

    if (scanf("%d", &number) != 1) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    printf("%s\n", is_even(number) ? "true" : "false");
    return EXIT_SUCCESS;
}