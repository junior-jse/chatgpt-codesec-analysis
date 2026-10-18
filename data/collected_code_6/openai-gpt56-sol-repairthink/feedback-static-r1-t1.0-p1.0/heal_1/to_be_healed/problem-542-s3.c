#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int replace_with_colon(char *string)
{
    regex_t regex;
    regmatch_t match;
    size_t offset = 0;
    int result;

    if (string == NULL) {
        return REG_BADPAT;
    }

    result = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (result != 0) {
        return result;
    }

    while (string[offset] != '\0') {
        result = regexec(&regex, string + offset, 1, &match, 0);
        if (result == REG_NOMATCH) {
            regfree(&regex);
            return 0;
        }

        if (result != 0) {
            regfree(&regex);
            return result;
        }

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            return REG_BADPAT;
        }

        offset += (size_t)match.rm_so;
        string[offset] = ':';
        ++offset;
    }

    regfree(&regex);
    return 0;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char string[1024];
    int result;

    if (fgets(string, sizeof(string), stdin) == NULL) {
        if (ferror(stdin)) {
            perror("fgets");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    string[strcspn(string, "\n")] = '\0';

    result = replace_with_colon(string);
    if (result != 0) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char error_message[256];

        regerror(result, NULL, error_message, sizeof(error_message));
        fprintf(stderr, "Regex error: %s\n", error_message);
        return EXIT_FAILURE;
    }

    if (puts(string) == EOF) {
        perror("puts");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}