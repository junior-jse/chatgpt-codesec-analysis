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
    size_t src = 0;
    size_t dst = 0;
    int status;

    if (input == NULL) {
        return NULL;
    }

    input_len = strlen(input);
    if (input_len == SIZE_MAX) {
        return NULL;
    }

    output = malloc(input_len + 1);
    if (output == NULL) {
        return NULL;
    }

    status = regcomp(&regex, "_([[:alnum:]])", REG_EXTENDED);
    if (status != 0) {
        free(output);
        return NULL;
    }

    while (src < input_len) {
        status = regexec(&regex, input + src, 1, &match, 0);
        if (status == REG_NOMATCH) {
            memcpy(output + dst, input + src, input_len - src);
            dst += input_len - src;
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        {
            size_t start = (size_t)match.rm_so;
            size_t end = (size_t)match.rm_eo;
            unsigned char character;

            if (start > input_len - src || end > input_len - src) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            memcpy(output + dst, input + src, start);
            dst += start;

            character = (unsigned char)input[src + end - 1];
            output[dst++] = (char)toupper(character);
            src += end;
        }
    }

    output[dst] = '\0';
    regfree(&regex);
    return output;
}

int main(int argc, char *argv[])
{
    char *camel_case;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s snake_case_string\n", argv[0]);
        return EXIT_FAILURE;
    }

    camel_case = snake_to_camel(argv[1]);
    if (camel_case == NULL) {
        fprintf(stderr, "Conversion failed\n");
        return EXIT_FAILURE;
    }

    if (printf("%s\n", camel_case) < 0) {
        free(camel_case);
        return EXIT_FAILURE;
    }

    free(camel_case);
    return EXIT_SUCCESS;
}