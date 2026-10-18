#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

char *camel_to_snake(const char *input)
{
    static const char *pattern = "([a-z0-9])([A-Z])";
    regex_t regex;
    regmatch_t matches[3];
    char *current = NULL;
    int status;

    if (input == NULL) {
        return NULL;
    }

    current = malloc(strlen(input) + 1);
    if (current == NULL) {
        return NULL;
    }
    strcpy(current, input);

    status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status != 0) {
        free(current);
        return NULL;
    }

    for (;;) {
        size_t length = strlen(current);
        size_t new_length;
        char *replacement;
        size_t prefix_length;
        size_t suffix_offset;

        status = regexec(&regex, current, 3, matches, 0);
        if (status == REG_NOMATCH) {
            break;
        }
        if (status != 0 || matches[0].rm_so < 0 || matches[0].rm_eo < 0 ||
            matches[1].rm_so < 0 || matches[2].rm_so < 0) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        if (length == SIZE_MAX) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        new_length = length + 1;
        replacement = malloc(new_length + 1);
        if (replacement == NULL) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        prefix_length = (size_t)matches[1].rm_so;
        suffix_offset = (size_t)matches[0].rm_eo;

        memcpy(replacement, current, prefix_length);
        replacement[prefix_length] =
            current[(size_t)matches[1].rm_so];
        replacement[prefix_length + 1] = '_';
        replacement[prefix_length + 2] = (char)tolower(
            (unsigned char)current[(size_t)matches[2].rm_so]);
        memcpy(replacement + prefix_length + 3,
               current + suffix_offset,
               length - suffix_offset + 1);

        free(current);
        current = replacement;
    }

    regfree(&regex);

    for (size_t i = 0; current[i] != '\0'; ++i) {
        current[i] = (char)tolower((unsigned char)current[i]);
    }

    return current;
}

int main(int argc, char *argv[])
{
    char *result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camelCaseString>\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = camel_to_snake(argv[1]);
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