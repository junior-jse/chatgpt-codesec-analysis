#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

char* remove_lowercase_substrings(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, SIZE_MAX);
    char *output = malloc(input_len + 1);
    if (output == NULL) {
        return NULL;
    }

    regex_t regex;
    int result = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (result != 0) {
        free(output);
        return NULL;
    }

    size_t out_len = 0;
    size_t search_pos = 0;
    regmatch_t match;

    while (search_pos < input_len) {
        result = regexec(&regex, input + search_pos, 1, &match, 0);
        if (result == 0) {
            if (match.rm_so > 0) {
                for (regoff_t i = 0; i < match.rm_so; i++) {
                    if (out_len < input_len) {
                        output[out_len++] = input[search_pos + i];
                    }
                }
            }
            search_pos += match.rm_eo;
        } else if (result == REG_NOMATCH) {
            size_t remaining = input_len - search_pos;
            for (size_t i = 0; i < remaining; i++) {
                if (out_len < input_len) {
                    output[out_len++] = input[search_pos + i];
                }
            }
            break;
        } else {
            regfree(&regex);
            free(output);
            return NULL;
        }
    }

    regfree(&regex);
    output[out_len] = '\0';

    char *final_output = realloc(output, out_len + 1);
    if (final_output != NULL) {
        return final_output;
    }
    return output;
}

int main(void) {
    char *result = remove_lowercase_substrings("HelloWORLD123abcDEF");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }
    return 0;
}