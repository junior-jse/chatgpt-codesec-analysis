#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static bool is_even(long number)
{
    return number % 2L == 0L;
}

int main(int argc, char *argv[])
{
    char *end = NULL;
    long number;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    number = strtol(argv[1], &end, 10);

    if (errno == ERANGE || end == argv[1] || *end != '\0') {
        fprintf(stderr, "Invalid integer: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("%s\n", is_even(number) ? "true" : "false");

    return EXIT_SUCCESS;
}