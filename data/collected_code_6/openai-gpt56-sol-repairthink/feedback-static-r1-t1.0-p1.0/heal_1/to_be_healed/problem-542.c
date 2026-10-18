#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int replace_delimiters(char *string)
{
    regex_t regex;
    regmatch_t match;
    size_t offset = 0;
    int status;

    if (string == NULL) {
        return -1;
    }

    status = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (status != 0) {
        return -1;
    }

    while ((status = regexec(&regex, string + offset, 1, &match, 0)) == 0) {
        if (match.rm_so < 0) {
            regfree(&regex);
            return -1;
        }

        offset += (size_t)match.rm_so;
        string[offset] = ':';
        ++offset;
    }

    regfree(&regex);

    return status == REG_NOMATCH ? 0 : -1;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    char *string;
    size_t length;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(argv[1]);
    string = malloc(length + 1);
    if (string == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(string, argv[1], length + 1);

    if (replace_delimiters(string) != 0) {
        fprintf(stderr, "Replacement failed\n");
        free(string);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", string) < 0) {
        free(string);
        return EXIT_FAILURE;
    }

    free(string);
    return EXIT_SUCCESS;
}