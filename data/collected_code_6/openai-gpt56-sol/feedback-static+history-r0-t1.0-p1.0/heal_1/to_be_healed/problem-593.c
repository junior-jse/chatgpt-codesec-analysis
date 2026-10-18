#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int remove_leading_zeroes(const char *input, char *output, size_t output_size)
{
    const char *cursor;
    size_t written = 0;

    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }

    cursor = input;

    for (int part = 0; part < 4; ++part) {
        char *end = NULL;
        unsigned long value;

        if (*cursor == '\0') {
            return -1;
        }

        for (const char *p = cursor; *p != '\0' && *p != '.'; ++p) {
            if (*p < '0' || *p > '9') {
                return -1;
            }
        }

        errno = 0;
        value = strtoul(cursor, &end, 10);

        if (errno != 0 || end == cursor || value > 255) {
            return -1;
        }

        if ((part < 3 && *end != '.') || (part == 3 && *end != '\0')) {
            return -1;
        }

        /* Possible weaknesses found:
         * Flawfinder snprintf: If format strings can be influenced by an attacker, they can be exploited, and note that sprintf variations do not always \0-terminate (CWE-134). Use a constant for the format specification. (risk 4, format)
         */
        int count = snprintf(output + written, output_size - written,
                             part == 0 ? "%lu" : ".%lu", value);

        if (count < 0 || (size_t)count >= output_size - written) {
            return -1;
        }

        written += (size_t)count;
        cursor = part < 3 ? end + 1 : end;
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
    char output[16];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPv4 address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (remove_leading_zeroes(argv[1], output, sizeof(output)) != 0) {
        fprintf(stderr, "Invalid IPv4 address\n");
        return EXIT_FAILURE;
    }

    if (printf("%s\n", output) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}