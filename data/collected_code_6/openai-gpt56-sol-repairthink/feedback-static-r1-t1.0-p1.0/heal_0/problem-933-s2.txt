#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int camel_to_snake(const char *input, char **output)
{
    regex_t boundary;
    regmatch_t match;
    char *result;
    size_t input_len;
    size_t capacity;
    size_t source_pos = 0U;
    size_t dest_pos = 0U;
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

    status = regcomp(&boundary, "([a-z0-9])([A-Z])", REG_EXTENDED);
    if (status != 0) {
        free(result);
        return -1;
    }

    while (source_pos < input_len) {
        status = regexec(&boundary, input + source_pos, 1U, &match, 0);
        if (status == REG_NOMATCH) {
            while (source_pos < input_len) {
                result[dest_pos++] =
                    (char)tolower((unsigned char)input[source_pos++]);
            }
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo < 0) {
            regfree(&boundary);
            free(result);
            return -1;
        }

        while (source_pos < input_len &&
               source_pos < source_pos + (size_t)match.rm_so) {
            result[dest_pos++] =
                (char)tolower((unsigned char)input[source_pos++]);
        }

        result[dest_pos++] =
            (char)tolower((unsigned char)input[source_pos++]);
        result[dest_pos++] = '_';
    }

    result[dest_pos] = '\0';
    regfree(&boundary);
    *output = result;
    return 0;
}

int main(int argc, char *argv[])
{
    char *snake_case = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camelCaseString>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (camel_to_snake(argv[1], &snake_case) != 0) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    puts(snake_case);
    free(snake_case);
    return EXIT_SUCCESS;
}