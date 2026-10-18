#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int remove_non_alphanumeric(const char *input, char **output)
{
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t input_len;
    size_t read_pos = 0;
    size_t write_pos = 0;
    int status;

    if (input == NULL || output == NULL) {
        return -1;
    }

    *output = NULL;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_len = strlen(input);

    if (input_len == SIZE_MAX) {
        return -1;
    }

    result = malloc(input_len + 1);
    if (result == NULL) {
        return -1;
    }

    status = regcomp(&regex, "[^[:alnum:]]+", REG_EXTENDED);
    if (status != 0) {
        free(result);
        return -1;
    }

    while (read_pos < input_len) {
        status = regexec(&regex, input + read_pos, 1, &match, 0);

        if (status == REG_NOMATCH) {
            size_t remaining = input_len - read_pos;
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + write_pos, input + read_pos, remaining);
            write_pos += remaining;
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            free(result);
            return -1;
        }

        {
            size_t prefix_len = (size_t)match.rm_so;
            size_t match_end = (size_t)match.rm_eo;

            if (match_end > input_len - read_pos) {
                regfree(&regex);
                free(result);
                return -1;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + write_pos, input + read_pos, prefix_len);
            write_pos += prefix_len;
            read_pos += match_end;
        }
    }

    result[write_pos] = '\0';
    regfree(&regex);
    *output = result;
    return 0;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[4096];
    char *output = NULL;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        int ch;

        do {
            /* Possible weaknesses found:
             * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
             */
            ch = getchar();
        } while (ch != '\n' && ch != EOF);

        if (ch == EOF && ferror(stdin)) {
            return EXIT_FAILURE;
        }

        return EXIT_FAILURE;
    }

    if (remove_non_alphanumeric(input, &output) != 0) {
        return EXIT_FAILURE;
    }

    if (puts(output) == EOF) {
        free(output);
        return EXIT_FAILURE;
    }

    free(output);
    return EXIT_SUCCESS;
}