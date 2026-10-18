#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int remove_leading_zeroes(const char *input, char *output, size_t output_size)
{
    const char *p = input;
    size_t offset = 0;

    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }

    for (int part = 0; part < 4; ++part) {
        char *end = NULL;
        unsigned long value;
        int written;

        if (*p == '\0') {
            return -1;
        }

        errno = 0;
        value = strtoul(p, &end, 10);

        if (errno != 0 || end == p || value > 255) {
            return -1;
        }

        if ((part < 3 && *end != '.') || (part == 3 && *end != '\0')) {
            return -1;
        }

        /* Possible weaknesses found:
         * Flawfinder snprintf: If format strings can be influenced by an attacker, they can be exploited, and note that sprintf variations do not always \0-terminate (CWE-134). Use a constant for the format specification. (risk 4, format)
         */
        written = snprintf(output + offset, output_size - offset,
                           part == 0 ? "%lu" : ".%lu", value);

        if (written < 0 || (size_t)written >= output_size - offset) {
            return -1;
        }

        offset += (size_t)written;
        p = part < 3 ? end + 1 : end;
    }

    return 0;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[16];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPv4 address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (remove_leading_zeroes(argv[1], result, sizeof(result)) != 0) {
        fprintf(stderr, "Invalid IPv4 address\n");
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}