#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* remove_lowercase_substrings(const char *input) {
    regex_t regex;
    regmatch_t match;
    int result;
    size_t input_len = strlen(input);
    size_t capacity = input_len + 1;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t out_len = 0;
    result = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (result != 0) {
        free(output);
        return NULL;
    }
    size_t search_pos = 0;
    while (search_pos < input_len) {
        result = regexec(&regex, input + search_pos, 1, &match, 0);
        if (result == 0) {
            if (match.rm_so > 0) {
                size_t copy_len = match.rm_so;
                if (out_len + copy_len >= capacity) {
                    capacity = (out_len + copy_len) * 2;
                    char *temp = realloc(output, capacity);
                    if (temp == NULL) {
                        regfree(&regex);
                        free(output);
                        return NULL;
                    }
                    output = temp;
                }
                memcpy(output + out_len, input + search_pos, copy_len);
                out_len += copy_len;
            }
            search_pos += match.rm_eo;
        } else if (result == REG_NOMATCH) {
            size_t remaining = input_len - search_pos;
            if (remaining > 0) {
                if (out_len + remaining >= capacity) {
                    capacity = out_len + remaining + 1;
                    char *temp = realloc(output, capacity);
                    if (temp == NULL) {
                        regfree(&regex);
                        free(output);
                        return NULL;
                    }
                    output = temp;
                }
                memcpy(output + out_len, input + search_pos, remaining);
                out_len += remaining;
            }
            search_pos = input_len;
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