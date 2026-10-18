#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static int split_at_uppercase(const char *input)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    int status;

    if (input == NULL) {
        return EXIT_FAILURE;
    }

    status = regcomp(&regex, "[[:upper:]]", REG_EXTENDED);
    if (status != 0) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char message[256];
        regerror(status, &regex, message, sizeof(message));
        fprintf(stderr, "regcomp failed: %s\n", message);
        return EXIT_FAILURE;
    }

    cursor = input;

    while (*cursor != '\0') {
        status = regexec(&regex, cursor, 1, &match, 0);

        if (status == REG_NOMATCH) {
            if (fputs(cursor, stdout) == EOF || fputc('\n', stdout) == EOF) {
                regfree(&regex);
                return EXIT_FAILURE;
            }
            break;
        }

        if (status != 0) {
            /* Possible weaknesses found:
             * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
             */
            char message[256];
            regerror(status, &regex, message, sizeof(message));
            fprintf(stderr, "regexec failed: %s\n", message);
            regfree(&regex);
            return EXIT_FAILURE;
        }

        if (match.rm_so > 0) {
            if (fwrite(cursor, 1, (size_t)match.rm_so, stdout) !=
                    (size_t)match.rm_so ||
                fputc('\n', stdout) == EOF) {
                regfree(&regex);
                return EXIT_FAILURE;
            }
        }

        cursor += match.rm_so;

        if (fputc(*cursor, stdout) == EOF) {
            regfree(&regex);
            return EXIT_FAILURE;
        }

        ++cursor;

        while (*cursor != '\0') {
            status = regexec(&regex, cursor, 1, &match, 0);
            if (status == REG_NOMATCH || match.rm_so > 0) {
                break;
            }
            if (status != 0) {
                /* Possible weaknesses found:
                 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
                 */
                char message[256];
                regerror(status, &regex, message, sizeof(message));
                fprintf(stderr, "regexec failed: %s\n", message);
                regfree(&regex);
                return EXIT_FAILURE;
            }
        }

        if (status == REG_NOMATCH) {
            if (fputs(cursor, stdout) == EOF || fputc('\n', stdout) == EOF) {
                regfree(&regex);
                return EXIT_FAILURE;
            }
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            cursor += strlen(cursor);
        } else if (fputc('\n', stdout) == EOF) {
            regfree(&regex);
            return EXIT_FAILURE;
        }
    }

    regfree(&regex);
    return ferror(stdout) ? EXIT_FAILURE : EXIT_SUCCESS;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return split_at_uppercase(argv[1]);
}