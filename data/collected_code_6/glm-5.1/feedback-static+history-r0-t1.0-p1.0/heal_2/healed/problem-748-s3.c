#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *put_spaces(const char *str) {
    if (!str) return NULL;

    size_t str_len = strlen(str);

    size_t capacity = str_len * 2 + 1;
    char *result = malloc(capacity);
    if (!result) return NULL;

    size_t result_len = 0;
    regex_t regex;
    int err = regcomp(&regex, "[A-Z][a-z]*", REG_EXTENDED);
    if (err != 0) {
        free(result);
        return NULL;
    }

    const char *p = str;

    while (1) {
        regmatch_t match;
        err = regexec(&regex, p, 1, &match, 0);
        if (err == REG_NOMATCH || match.rm_so == -1) break;
        if (err != 0) {
            regfree(&regex);
            free(result);
            return NULL;
        }

        size_t prefix_len = match.rm_so;
        size_t match_len = match.rm_eo - match.rm_so;

        if (result_len + prefix_len + match_len + 2 >= capacity) {
            capacity = (result_len + prefix_len + match_len + 2) * 2;
            char *temp = realloc(result, capacity);
            if (!temp) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            result = temp;
        }

        if (prefix_len > 0) {
            memcpy(result + result_len, p, prefix_len);
            result_len += prefix_len;
        }

        if (result_len > 0) {
            result[result_len++] = ' ';
        }

        memcpy(result + result_len, p + match.rm_so, match_len);
        result_len += match_len;

        p += match.rm_eo;
    }

    size_t remaining_len = strlen(p);

    if (remaining_len > 0) {
        if (result_len + remaining_len + 1 >= capacity) {
            capacity = result_len + remaining_len + 2;
            char *temp = realloc(result, capacity);
            if (!temp) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            result = temp;
        }
        memcpy(result + result_len, p, remaining_len);
        result_len += remaining_len;
    }

    result[result_len] = '\0';

    regfree(&regex);

    char *final_result = realloc(result, result_len + 1);
    if (!final_result) return result;

    return final_result;
}

int main(int argc, char * const argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char *output = put_spaces(argv[1]);
    if (!output) {
        fprintf(stderr, "Memory or regex error\n");
        return 1;
    }

    printf("%s\n", output);

    free(output);
    return 0;
}