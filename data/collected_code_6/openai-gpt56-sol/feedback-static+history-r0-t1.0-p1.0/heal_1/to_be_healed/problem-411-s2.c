#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *snake_to_camel(const char *input)
{
    regex_t regex;
    regmatch_t match;
    char *output;
    size_t input_len;
    size_t read_pos = 0;
    size_t write_pos = 0;
    int status;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_len = strlen(input);
    if (input_len == SIZE_MAX) {
        return NULL;
    }

    output = malloc(input_len + 1);
    if (output == NULL) {
        return NULL;
    }

    status = regcomp(&regex, "_+([[:alnum:]])", REG_EXTENDED);
    if (status != 0) {
        free(output);
        return NULL;
    }

    while (read_pos < input_len) {
        status = regexec(&regex, input + read_pos, 2, &match, 0);
        if (status == REG_NOMATCH) {
            size_t remaining = input_len - read_pos;

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + write_pos, input + read_pos, remaining);
            write_pos += remaining;
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        {
            size_t prefix_len = (size_t)match.rm_so;
            size_t matched_len = (size_t)match.rm_eo;

            if (prefix_len > input_len - read_pos ||
                matched_len > input_len - read_pos) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + write_pos, input + read_pos, prefix_len);
            write_pos += prefix_len;
            output[write_pos++] = (char)toupper(
                (unsigned char)input[read_pos + matched_len - 1]
            );
            read_pos += matched_len;
        }
    }

    output[write_pos] = '\0';
    regfree(&regex);
    return output;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    char *result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s snake_case_string\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = snake_to_camel(argv[1]);
    if (result == NULL) {
        fprintf(stderr, "Conversion failed\n");
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}