#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *add_spaces_capitals(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    regex_t regex;
    int reti = regcomp(&regex, "([A-Z][a-z]*)", REG_EXTENDED);
    if (reti != 0) {
        return NULL;
    }

    size_t len = strlen(str);

    size_t max_len = len * 2 + 2;
    char *result = malloc(max_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t res_idx = 0;
    const char *p = str;

    while (*p) {
        regmatch_t match;
        reti = regexec(&regex, p, 1, &match, 0);
        if (reti == REG_NOMATCH) {
            size_t remaining = strlen(p);
            if (remaining > 0) {
                size_t required = res_idx + remaining;
                if (required >= max_len) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                memcpy(result + res_idx, p, remaining);
                res_idx += remaining;
            }
            break;
        } else if (reti != 0) {
            free(result);
            regfree(&regex);
            return NULL;
        } else {
            if (match.rm_so > 0) {
                size_t required = res_idx + match.rm_so;
                if (required >= max_len) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                memcpy(result + res_idx, p, match.rm_so);
                res_idx += match.rm_so;
            }
            if (p != str && res_idx > 0) {
                size_t required = res_idx + 1;
                if (required >= max_len) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result[res_idx++] = ' ';
            }
            size_t match_len = match.rm_eo - match.rm_so;
            size_t required = res_idx + match_len;
            if (required >= max_len) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            memcpy(result + res_idx, p + match.rm_so, match_len);
            res_idx += match_len;
            p += match.rm_eo;
        }
    }

    if (res_idx >= max_len) {
        free(result);
        regfree(&regex);
        return NULL;
    }
    result[res_idx] = '\0';

    regfree(&regex);
    return result;
}

int main(int argc, char const * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char *res = add_spaces_capitals(argv[1]);
    if (res == NULL) {
        fprintf(stderr, "Error processing string\n");
        return 1;
    }

    printf("%s\n", res);
    free(res);

    return 0;
}