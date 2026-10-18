#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *space_out_capitals(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "([A-Z][a-z0-9]+)", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t str_len = strlen(str);

    size_t max_len = str_len * 2 + 1;
    char *result = malloc(max_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    const char *p = str;
    char *out = result;
    const char * const out_end = result + max_len;

    regmatch_t match[2];
    while (out < out_end) {
        ret = regexec(&regex, p, 2, match, 0);
        if (ret != 0) {
            break;
        }

        if (match[1].rm_so == -1) {
            break;
        }

        size_t prefix_len = match[1].rm_so;
        if (prefix_len > 0) {
            if (out + prefix_len >= out_end) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(out, p, prefix_len);
            out += prefix_len;
            if (out >= out_end) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            *out = ' ';
            out++;
            p += match[1].rm_so;
        } else {
            if (p != str) {
                if (out >= out_end) {
                    regfree(&regex);
                    free(result);
                    return NULL;
                }
                *out = ' ';
                out++;
            }
            p += match[1].rm_eo;
        }

        size_t word_len = match[1].rm_eo - match[1].rm_so;
        if (word_len > 0) {
            if (out + word_len >= out_end) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(out, p, word_len);
            out += word_len;
            p += match[1].rm_eo - match[1].rm_so;
        } else {
            p++;
        }
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t remaining_len = strlen(p);

    if (out + remaining_len >= out_end) {
        regfree(&regex);
        free(result);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(out, p, remaining_len);
    out += remaining_len;
    *out = '\0';

    regfree(&regex);

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t final_len = strlen(result) + 1;

    char *shrunk = realloc(result, final_len);
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

    /* Possible weaknesses found:
     *  Assignment 'res4=space_out_capitals(NULL)', assigned value is 0
     *  Calling function 'space_out_capitals' returns 0
     */
    const char * const res4 = space_out_capitals(NULL);
    /* Possible weaknesses found:
     *  Condition 'res4==NULL' is always true
     *  Condition 'res4==NULL' is always true [knownConditionTrueFalse]
     */
    if (res4 == NULL) {
        printf("NULL handled\n");
    }

    return 0;
}