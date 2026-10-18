#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int is_prime(uint64_t value)
{
    uint64_t divisor;

    if (value < 2U) {
        return 0;
    }

    if ((value & 1U) == 0U) {
        return value == 2U;
    }

    for (divisor = 3U; divisor <= value / divisor; divisor += 2U) {
        if (value % divisor == 0U) {
            return 0;
        }
    }

    return 1;
}

static int nth_newman_shanks_williams_prime(uint64_t n, uint64_t *result)
{
    uint64_t previous = 1U;
    uint64_t current = 1U;
    uint64_t count = 0U;

    if (n == 0U || result == NULL) {
        return 0;
    }

    for (;;) {
        uint64_t next;

        if (current > (UINT64_MAX - previous) / 6U) {
            return 0;
        }

        next = 6U * current - previous;
        previous = current;
        current = next;

        if (is_prime(current)) {
            ++count;
            if (count == n) {
                *result = current;
                return 1;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    char *end = NULL;
    uintmax_t input;
    uint64_t result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    input = strtoumax(argv[1], &end, 10);

    if (errno == ERANGE || end == argv[1] || *end != '\0' ||
        input == 0U || input > UINT64_MAX) {
        fprintf(stderr, "Invalid n\n");
        return EXIT_FAILURE;
    }

    if (!nth_newman_shanks_williams_prime((uint64_t)input, &result)) {
        fprintf(stderr, "Result exceeds the supported range\n");
        return EXIT_FAILURE;
    }

    printf("%" PRIu64 "\n", result);
    return EXIT_SUCCESS;
}