#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_lowercase_substrings(const char *input) {
    regex_t regex;
    int status;
    size_t max_matches = 1;
    regmatch_t match;
    char *result = (char *)malloc(strlen(input) + 1);

    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';

    status = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (status != 0) {
        free(result);
        regfree(&regex);
        return NULL;
    }

    const char *cursor = input;
    size_t current_pos = 0;

    while ((status = regexec(&regex, cursor, max_matches, &match, 0)) == 0) {
        if (match.rm_so > 0) {
            strncat(result, cursor, match.rm_so);
        }
        cursor += match.rm_eo;
    }

    if (status == REG_NOMATCH) {
        strcat(result, cursor);
    } else {
        free(result);
        regfree(&regex);
        return NULL;
    }

    regfree(&regex);

    char *trimmed_result = (char *)realloc(result, strlen(result) + 1);
    if (trimmed_result != NULL) {
        return trimmed_result;
    }

    return result;
}

int main(void) {
    const char *input_string = "HelloWorld123abcDEF456ghiJKL";
    char *output_string = remove_lowercase_substrings(input_string);

    if (output_string != NULL) {
        printf("Input: %s\n", input_string);
        printf("Output: %s\n", output_string);
        free(output_string);
    } else {
        printf("Memory allocation or regex error occurred.\n");
    }

    return 0;
}