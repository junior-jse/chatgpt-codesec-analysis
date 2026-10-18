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
    size_t input_length;
    size_t read_pos = 0;
    size_t write_pos = 0;
    int regex_status;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_length = strlen(input);
    if (input_length == SIZE_MAX) {
        return NULL;
    }

    output = malloc(input_length + 1);
    if (output == NULL) {
        return NULL;
    }

    regex_status = regcomp(&regex, "_([[:alnum:]])", REG_EXTENDED);
    if (regex_status != 0) {
        free(output);
        return NULL;
    }

    while (read_pos < input_length) {
        regex_status = regexec(&regex, input + read_pos, 1, &match, 0);

        if (regex_status == REG_NOMATCH) {
            size_t remaining = input_length - read_pos;
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + write_pos, input + read_pos, remaining);
            write_pos += remaining;
            break;
        }

        if (regex_status != 0 || match.rm_so < 0 || match.rm_eo < 0) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        size_t match_start = (size_t)match.rm_so;
        size_t match_end = (size_t)match.rm_eo;

        if (match_end <= match_start || match_end > input_length - read_pos) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(output + write_pos, input + read_pos, match_start);
        write_pos += match_start;
        output[write_pos++] = (char)toupper(
            (unsigned char)input[read_pos + match_end - 1]
        );
        read_pos += match_end;
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