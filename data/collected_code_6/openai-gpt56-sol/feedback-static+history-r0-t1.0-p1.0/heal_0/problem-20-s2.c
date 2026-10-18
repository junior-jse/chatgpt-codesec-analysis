#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static bool is_woodall(uint64_t value)
{
    uint64_t power_of_two = 2U;

    for (uint64_t n = 1U; ; ++n) {
        if (n <= UINT64_MAX / power_of_two) {
            const uint64_t product = n * power_of_two;

            if (product > 0U) {
                const uint64_t woodall = product - 1U;

                if (value == woodall) {
                    return true;
                }

                if (woodall > value) {
                    return false;
                }
            }
        } else {
            return false;
        }

        if (power_of_two > UINT64_MAX / 2U) {
            return false;
        }

        power_of_two *= 2U;
    }
}

int main(int argc, char *argv[])
{
    char *end = NULL;
    uintmax_t parsed_value;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <non-negative integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    parsed_value = strtoumax(argv[1], &end, 10);

    if (errno == ERANGE || end == argv[1] || *end != '\0' ||
        parsed_value > UINT64_MAX || argv[1][0] == '-') {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", is_woodall((uint64_t)parsed_value) ? "Woodall" : "Not Woodall");
    return EXIT_SUCCESS;
}