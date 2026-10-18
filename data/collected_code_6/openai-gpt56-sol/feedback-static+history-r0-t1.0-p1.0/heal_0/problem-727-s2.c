#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int remove_non_alphanumeric(const char *input, char **output)
{
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t input_length;
    size_t read_pos = 0;
    size_t write_pos = 0;
    int status;

    if (input == NULL || output == NULL) {
        return -1;
    }

    *output = NULL;
    input_length = strlen(input);

    if (input_length == SIZE_MAX) {
        return -1;
    }

    result = malloc(input_length + 1);
    if (result == NULL) {
        return -1;
    }

    status = regcomp(&regex, "[^[:alnum:]]+", REG_EXTENDED);
    if (status != 0) {
        free(result);
        return -1;
    }

    while (read_pos < input_length) {
        status = regexec(&regex, input + read_pos, 1, &match, 0);

        if (status == REG_NOMATCH) {
            size_t remaining = input_length - read_pos;
            memcpy(result + write_pos, input + read_pos, remaining);
            write_pos += remaining;
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo < match.rm_so) {
            regfree(&regex);
            free(result);
            return -1;
        }

        {
            size_t prefix_length = (size_t)match.rm_so;
            size_t matched_length = (size_t)(match.rm_eo - match.rm_so);

            if (prefix_length > input_length - read_pos ||
                matched_length > input_length - read_pos - prefix_length) {
                regfree(&regex);
                free(result);
                return -1;
            }

            memcpy(result + write_pos, input + read_pos, prefix_length);
            write_pos += prefix_length;
            read_pos += prefix_length + matched_length;
        }
    }

    result[write_pos] = '\0';
    regfree(&regex);
    *output = result;
    return 0;
}

int main(void)
{
    const char input[] = "Hello, World! 123_@#";
    char *output = NULL;

    if (remove_non_alphanumeric(input, &output) != 0) {
        fputs("Failed to process input.\n", stderr);
        return EXIT_FAILURE;
    }

    if (puts(output) == EOF) {
        free(output);
        return EXIT_FAILURE;
    }

    free(output);
    return EXIT_SUCCESS;
}