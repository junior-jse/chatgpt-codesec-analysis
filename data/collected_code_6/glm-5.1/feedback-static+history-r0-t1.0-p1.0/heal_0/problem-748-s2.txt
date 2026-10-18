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
    char *result = malloc(len * 2 + 2);
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
            strcpy(result + res_idx, p);
            break;
        } else if (reti != 0) {
            free(result);
            regfree(&regex);
            return NULL;
        } else {
            if (match.rm_so > 0) {
                memcpy(result + res_idx, p, match.rm_so);
                res_idx += match.rm_so;
            }
            if (p != str && res_idx > 0) {
                result[res_idx++] = ' ';
            }
            size_t match_len = match.rm_eo - match.rm_so;
            memcpy(result + res_idx, p + match.rm_so, match_len);
            res_idx += match_len;
            p += match.rm_eo;
        }
    }

    result[res_idx] = '\0';

    regfree(&regex);
    return result;
}

int main(int argc, char *argv[]) {
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