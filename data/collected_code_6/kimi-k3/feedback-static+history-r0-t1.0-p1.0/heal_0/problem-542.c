#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *replace_with_colon(const char *input) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[ ,.]";
    char *result;
    size_t input_len;
    size_t result_len = 0;
    size_t i = 0;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    input_len = strlen(input);
    result = malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return NULL;
    }

    while (i < input_len) {
        ret = regexec(&regex, input + i, 1, &match, 0);
        if (ret == 0 && match.rm_so == 0) {
            result[result_len++] = ':';
            i += (size_t)match.rm_eo;
        } else {
            result[result_len++] = input[i++];
        }
    }

    result[result_len] = '\0';
    regfree(&regex);

    return result;
}

int main(void) {
    const char *test_string = "Hello, world. This is a test";
    char *modified;

    modified = replace_with_colon(test_string);
    if (modified == NULL) {
        fprintf(stderr, "Error: Failed to process string\n");
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", test_string);
    printf("Modified: %s\n", modified);

    free(modified);
    return EXIT_SUCCESS;
}