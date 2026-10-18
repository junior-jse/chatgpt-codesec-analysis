#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int is_prime(uint64_t value)
{
    uint64_t divisor;

    if (value < 2) {
        return 0;
    }

    if ((value & UINT64_C(1)) == 0) {
        return value == 2;
    }

    for (divisor = 3; divisor <= value / divisor; divisor += 2) {
        if (value % divisor == 0) {
            return 0;
        }
    }

    return 1;
}

static int nth_newman_shanks_williams_prime(size_t n, uint64_t *result)
{
    uint64_t previous = 1;
    uint64_t current = 1;
    size_t count = 0;

    if (n == 0 || result == NULL) {
        return 0;
    }

    for (;;) {
        uint64_t next;

        if (is_prime(previous)) {
            ++count;
            if (count == n) {
                *result = previous;
                return 1;
            }
        }

        if (current > (UINT64_MAX - previous) / 6) {
            return 0;
        }

        next = 6 * current - previous;
        previous = current;
        current = next;
    }
}

int main(int argc, char *argv[])
{
    char *end = NULL;
    uintmax_t parsed;
    uint64_t result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    parsed = strtoumax(argv[1], &end, 10);

    if (errno != 0 || end == argv[1] || *end != '\0' ||
        parsed == 0 || parsed > SIZE_MAX) {
        fprintf(stderr, "Invalid n\n");
        return EXIT_FAILURE;
    }

    if (!nth_newman_shanks_williams_prime((size_t)parsed, &result)) {
        fprintf(stderr, "Unable to compute the requested prime\n");
        return EXIT_FAILURE;
    }

    if (printf("%" PRIu64 "\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}