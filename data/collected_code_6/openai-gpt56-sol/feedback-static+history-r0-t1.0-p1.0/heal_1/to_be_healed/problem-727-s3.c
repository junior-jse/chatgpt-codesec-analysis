#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_non_alphanumeric(const char *input)
{
    regex_t regex;
    regmatch_t match;
    char *output;
    size_t input_length;
    size_t read_position = 0;
    size_t write_position = 0;
    int status;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_length = strlen(input);
    output = malloc(input_length + 1);
    if (output == NULL) {
        return NULL;
    }

    status = regcomp(&regex, "[^[:alnum:]]+", REG_EXTENDED);
    if (status != 0) {
        free(output);
        return NULL;
    }

    while (read_position < input_length) {
        status = regexec(&regex, input + read_position, 1, &match, 0);

        if (status == REG_NOMATCH) {
            size_t remaining = input_length - read_position;
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + write_position, input + read_position, remaining);
            write_position += remaining;
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo < match.rm_so) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        size_t prefix_length = (size_t)match.rm_so;
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(output + write_position, input + read_position, prefix_length);
        write_position += prefix_length;
        read_position += (size_t)match.rm_eo;
    }

    output[write_position] = '\0';
    regfree(&regex);
    return output;
}

int main(void)
{
    const char input[] = "Hello, World! 123_+-";
    char *result = remove_non_alphanumeric(input);

    if (result == NULL) {
        fputs("Failed to process input.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}