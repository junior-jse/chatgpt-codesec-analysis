#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int find_sequences(const char *input)
{
    const char *pattern = "(^|[^[:alpha:]])([[:upper:]][[:lower:]]+)";
    regex_t regex;
    regmatch_t matches[3];
    const char *cursor = input;
    int status;
    int found = 0;

    status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status != 0) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char error_message[256];
        regerror(status, &regex, error_message, sizeof(error_message));
        fprintf(stderr, "Regex compilation failed: %s\n", error_message);
        return -1;
    }

    while ((status = regexec(&regex, cursor, 3, matches, 0)) == 0) {
        size_t start;
        size_t length;

        if (matches[2].rm_so < 0 || matches[2].rm_eo < matches[2].rm_so) {
            regfree(&regex);
            return -1;
        }

        start = (size_t)matches[2].rm_so;
        length = (size_t)(matches[2].rm_eo - matches[2].rm_so);

        if (printf("%.*s\n", (int)length, cursor + start) < 0) {
            regfree(&regex);
            return -1;
        }

        found = 1;
        cursor += matches[2].rm_eo;
    }

    if (status != REG_NOMATCH) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char error_message[256];
        regerror(status, &regex, error_message, sizeof(error_message));
        fprintf(stderr, "Regex matching failed: %s\n", error_message);
        regfree(&regex);
        return -1;
    }

    regfree(&regex);
    return found;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    int result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"string\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = find_sequences(argv[1]);
    if (result < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}