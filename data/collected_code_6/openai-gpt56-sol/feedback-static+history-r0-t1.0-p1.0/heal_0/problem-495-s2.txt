#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static int remove_lowercase_substrings(const char *input, char **output)
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
    input_len = strlen(input);

    if (input_len == SIZE_MAX) {
        return -1;
    }

    result = malloc(input_len + 1);
    if (result == NULL) {
        return -1;
    }

    status = regcomp(&regex, "[[:lower:]]+", REG_EXTENDED);
    if (status != 0) {
        free(result);
        return -1;
    }

    while (read_pos < input_len) {
        status = regexec(&regex, input + read_pos, 1, &match, 0);

        if (status == REG_NOMATCH) {
            size_t remaining = input_len - read_pos;

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
    char input[4096];
    char *output = NULL;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    input[strcspn(input, "\n")] = '\0';

    if (remove_lowercase_substrings(input, &output) != 0) {
        fprintf(stderr, "Failed to process input.\n");
        return EXIT_FAILURE;
    }

    if (puts(output) == EOF) {
        free(output);
        fprintf(stderr, "Failed to write output.\n");
        return EXIT_FAILURE;
    }

    free(output);
    return EXIT_SUCCESS;
}