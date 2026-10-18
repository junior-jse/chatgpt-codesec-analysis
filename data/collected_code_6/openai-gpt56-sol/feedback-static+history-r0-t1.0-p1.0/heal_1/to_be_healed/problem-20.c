#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static bool is_woodall(uint64_t number)
{
    uint64_t power_of_two = 2U;

    for (uint64_t n = 1U; n <= number; ++n) {
        if (n > UINT64_MAX / power_of_two) {
            return false;
        }

        uint64_t product = n * power_of_two;

        if (product > 0U && product - 1U == number) {
            return true;
        }

        if (product - 1U > number || power_of_two > UINT64_MAX / 2U) {
            return false;
        }

        power_of_two *= 2U;
    }

    return false;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[128];
    char *end = NULL;

    if (fgets(input, sizeof input, stdin) == NULL) {
        return EXIT_FAILURE;
    }

    errno = 0;
    uintmax_t value = strtoumax(input, &end, 10);

    if (errno == ERANGE || end == input || value > UINT64_MAX) {
        return EXIT_FAILURE;
    }

    while (*end == ' ' || *end == '\t' || *end == '\n' ||
           *end == '\r' || *end == '\f' || *end == '\v') {
        ++end;
    }

    if (*end != '\0') {
        return EXIT_FAILURE;
    }

    printf("%" PRIuMAX " is %sa Woodall number.\n",
           value, is_woodall((uint64_t)value) ? "" : "not ");

    return EXIT_SUCCESS;
}