#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static void print_regex_error(int errcode, regex_t *regex, const char *prefix) {
    size_t error_size;
    char *error_buffer;

    error_size = regerror(errcode, regex, NULL, 0);
    error_buffer = malloc(error_size);
    if (error_buffer == NULL) {
        fprintf(stderr, "%s (error code %d)\n", prefix, errcode);
        return;
    }

    regerror(errcode, regex, error_buffer, error_size);
    fprintf(stderr, "%s: %s\n", prefix, error_buffer);
    free(error_buffer);
}

int find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match;
    int ret;
    const char *cursor = str;
    int found = 0;

    if (str == NULL) {
        fprintf(stderr, "Error: Input string is NULL\n");
        return -1;
    }

    ret = regcomp(&regex, "[A-Z][a-z]+", REG_EXTENDED);
    if (ret != 0) {
        print_regex_error(ret, &regex, "Error: Could not compile regex");
        return -1;
    }

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            print_regex_error(ret, &regex, "Error: Regex execution failed");
            regfree(&regex);
            return -1;
        }

        if (match.rm_so == -1) {
            break;
        }

        printf("Found sequence: %.*s\n",
               (int)(match.rm_eo - match.rm_so),
               cursor + match.rm_so);

        if (match.rm_eo == 0) {
            cursor++;
        } else {
            cursor += match.rm_eo;
        }
        found = 1;
    }

    if (!found) {
        printf("No sequences found\n");
    }

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_string = "Hello World This Is A Test String with MixedCase Words";

    printf("Searching in: \"%s\"\n", test_string);

    if (find_sequences(test_string) != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}