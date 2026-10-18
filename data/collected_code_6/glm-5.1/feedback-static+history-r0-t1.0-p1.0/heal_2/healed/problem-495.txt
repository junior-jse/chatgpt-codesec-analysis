#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_lowercase_substrings(const char *input) {
    if (input == NULL) return NULL;
    
    regex_t regex;
    int status;
    regmatch_t match;
    size_t input_len = 0;
    const char *p = input;
    while (*p != '\0') {
        input_len++;
        p++;
    }
    
    char *result = (char *)malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;

    status = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (status != 0) {
        free(result);
        return NULL;
    }

    const char *cursor = input;

    while ((status = regexec(&regex, cursor, 1, &match, 0)) == 0) {
        if (match.rm_so > 0) {
            if (result_len + match.rm_so <= input_len) {
                memcpy(result + result_len, cursor, match.rm_so);
                result_len += match.rm_so;
            }
        }
        cursor += match.rm_eo;
    }

    if (status == REG_NOMATCH) {
        size_t remaining_len = 0;
        const char *r = cursor;
        while (*r != '\0') {
            remaining_len++;
            r++;
        }
        if (result_len + remaining_len <= input_len) {
            memcpy(result + result_len, cursor, remaining_len);
            result_len += remaining_len;
        }
        result[result_len] = '\0';
    } else {
        free(result);
        regfree(&regex);
        return NULL;
    }

    regfree(&regex);

    char *trimmed_result = (char *)realloc(result, result_len + 1);
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