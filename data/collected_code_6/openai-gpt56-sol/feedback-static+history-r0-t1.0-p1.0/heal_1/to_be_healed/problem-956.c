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
        return -1;
    }

    status = regcomp(&regex, "[[:upper:]]", REG_EXTENDED);
    if (status != 0) {
        return -1;
    }

    cursor = input;

    while (*cursor != '\0') {
        status = regexec(&regex, cursor, 1, &match, 0);

        if (status == REG_NOMATCH) {
            if (printf("%s\n", cursor) < 0) {
                regfree(&regex);
                return -1;
            }
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            return -1;
        }

        if (match.rm_so > 0) {
            if (printf("%.*s\n", (int)match.rm_so, cursor) < 0) {
                regfree(&regex);
                return -1;
            }
        }

        cursor += match.rm_so;

        status = regexec(&regex, cursor + 1, 1, &match, 0);

        if (status == REG_NOMATCH) {
            if (printf("%s\n", cursor) < 0) {
                regfree(&regex);
                return -1;
            }
            break;
        }

        if (status != 0 || match.rm_so < 0) {
            regfree(&regex);
            return -1;
        }

        if (printf("%.*s\n", (int)(match.rm_so + 1), cursor) < 0) {
            regfree(&regex);
            return -1;
        }

        cursor += match.rm_so + 1;
    }

    regfree(&regex);
    return 0;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        if (fprintf(stderr, "Usage: %s <string>\n", argv[0]) < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (split_at_uppercase(argv[1]) != 0) {
        if (fprintf(stderr, "Failed to split string\n") < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}