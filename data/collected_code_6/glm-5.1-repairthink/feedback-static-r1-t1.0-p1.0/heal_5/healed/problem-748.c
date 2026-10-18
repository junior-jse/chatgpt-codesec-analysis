#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

char *space_out_capitals(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "([A-Z][a-z0-9]+)", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t str_len = strlen(str);
    if (str_len >= SIZE_MAX / 2) {
        regfree(&regex);
        return NULL;
    }
    
    size_t max_len = str_len * 2 + 1;
    char *result = malloc(max_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    const char *p = str;
    size_t out_len = 0;

    regmatch_t match[2];
    while (1) {
        ret = regexec(&regex, p, 2, match, 0);
        if (ret != 0) {
            break;
        }

        if (match[1].rm_so == -1) {
            break;
        }

        size_t prefix_len = match[1].rm_so;
        size_t word_len = match[1].rm_eo - match[1].rm_so;

        if (prefix_len > 0) {
            if (prefix_len > max_len - out_len) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            memcpy(result + out_len, p, prefix_len);
            out_len += prefix_len;
            
            if (1 > max_len - out_len) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            result[out_len++] = ' ';
        } else if (p != str) {
            if (1 > max_len - out_len) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            result[out_len++] = ' ';
        }

        if (word_len > 0) {
            if (word_len > max_len - out_len) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            memcpy(result + out_len, p + match[1].rm_so, word_len);
            out_len += word_len;
        }

        p += match[1].rm_eo;
        if (match[1].rm_eo == 0) {
            p++;
        }
    }

    size_t offset = p - str;
    size_t remaining_len = (offset < str_len) ? (str_len - offset) : 0;
    if (remaining_len > max_len - out_len) {
        regfree(&regex);
        free(result);
        return NULL;
    }
    memcpy(result + out_len, p, remaining_len);
    out_len += remaining_len;

    if (1 > max_len - out_len) {
        regfree(&regex);
        free(result);
        return NULL;
    }
    result[out_len] = '\0';

    regfree(&regex);

    char *shrunk = realloc(result, out_len + 1);
    if (shrunk != NULL) {
        return shrunk;
    }
    return result;
}

int main(void) {
    char *res1 = space_out_capitals("HelloWorldExample");
    if (res1 != NULL) {
        printf("%s\n", res1);
        free(res1);
    }

    char *res2 = space_out_capitals("ThisIsATest");
    if (res2 != NULL) {
        printf("%s\n", res2);
        free(res2);
    }

    char *res3 = space_out_capitals("Already Spaced");
    if (res3 != NULL) {
        printf("%s\n", res3);
        free(res3);
    }

    space_out_capitals(NULL);
    printf("NULL handled\n");

    return 0;
}