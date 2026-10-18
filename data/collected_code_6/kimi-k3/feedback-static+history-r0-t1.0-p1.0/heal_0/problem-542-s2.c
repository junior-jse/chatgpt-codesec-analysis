#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

int replace_with_colon(const char *input, char **output) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[ ,.]";
    size_t input_len;
    size_t result_len = 0;
    size_t i = 0;
    char *result;

    if (input == NULL || output == NULL) {
        return -1;
    }

    *output = NULL;
    input_len = strlen(input);

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return -1;
    }

    result = malloc(input_len + 1);
    if (result == NULL) {
        regfree(&regex);
        return -1;
    }

    while (i < input_len) {
        if (regexec(&regex, input + i, 1, &match, 0) == 0) {
            size_t match_start = i + (size_t)match.rm_so;
            size_t match_end = i + (size_t)match.rm_eo;

            while (i < match_start) {
                result[result_len++] = input[i++];
            }

            result[result_len++] = ':';
            i = match_end;
        } else {
            result[result_len++] = input[i++];
        }
    }

    result[result_len] = '\0';
    regfree(&regex);
    *output = result;
    return 0;
}

int main(void) {
    const char *input = "Hello, world. This is a test";
    char *output = NULL;

    if (replace_with_colon(input, &output) == 0) {
        printf("Original: %s\n", input);
        printf("Modified: %s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}