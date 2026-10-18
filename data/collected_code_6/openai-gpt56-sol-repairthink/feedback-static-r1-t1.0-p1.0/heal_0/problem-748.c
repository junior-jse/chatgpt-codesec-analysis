#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int add_spaces_before_capitals(const char *input, char **output)
{
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t input_len;
    size_t capacity;
    size_t read_pos = 0;
    size_t write_pos = 0;
    int status;

    if (input == NULL || output == NULL) {
        return -1;
    }

    *output = NULL;
    input_len = strlen(input);

    if (input_len > (SIZE_MAX - 1U) / 2U) {
        return -1;
    }

    capacity = input_len * 2U + 1U;
    result = malloc(capacity);
    if (result == NULL) {
        return -1;
    }

    status = regcomp(&regex, "[[:upper:]]", REG_EXTENDED);
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
            size_t match_pos = read_pos + (size_t)match.rm_so;
            size_t prefix_len = match_pos - read_pos;

            memcpy(result + write_pos, input + read_pos, prefix_len);
            write_pos += prefix_len;

            if (match_pos > 0U && input[match_pos - 1U] != ' ') {
                result[write_pos++] = ' ';
            }

            result[write_pos++] = input[match_pos];
            read_pos = match_pos + 1U;
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
    size_t length;

    if (fgets(input, sizeof input, stdin) == NULL) {
        return EXIT_FAILURE;
    }

    length = strlen(input);
    if (length > 0U && input[length - 1U] == '\n') {
        input[length - 1U] = '\0';
    } else if (length == sizeof input - 1U) {
        int ch = getchar();

        if (ch != '\n' && ch != EOF) {
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            return EXIT_FAILURE;
        }
    }

    if (add_spaces_before_capitals(input, &output) != 0) {
        return EXIT_FAILURE;
    }

    if (puts(output) == EOF) {
        free(output);
        return EXIT_FAILURE;
    }

    free(output);
    return EXIT_SUCCESS;
}