#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <regex.h>

#define MAX_INPUT_LEN 4096

char *add_spaces(const char *input, size_t max_len)
{
    regex_t regex;
    regmatch_t match;
    char *result = NULL;
    char *buf = NULL;
    size_t input_len = 0;
    size_t buf_size = 0;
    size_t result_size = 0;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    int reti;

    if (input == NULL || max_len == 0) {
        return NULL;
    }

    input_len = strnlen(input, max_len);
    if (input_len == 0) {
        result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    if (input_len > (SIZE_MAX - 1) / 2) {
        return NULL;
    }

    if (input_len >= SIZE_MAX - 1) {
        return NULL;
    }

    buf_size = input_len + 1;
    buf = malloc(buf_size);
    if (buf == NULL) {
        return NULL;
    }

    for (k = 0; k < input_len && k < buf_size - 1; k++) {
        buf[k] = input[k];
    }
    buf[input_len] = '\0';

    reti = regcomp(&regex, "[A-Z][a-z]*", REG_EXTENDED);
    if (reti != 0) {
        free(buf);
        return NULL;
    }

    result_size = (input_len * 2) + 1;
    result = malloc(result_size);
    if (result == NULL) {
        regfree(&regex);
        free(buf);
        return NULL;
    }

    /* Possible weaknesses found:
     *  Assuming that condition 'j+1<result_size' is not redundant
     */
    while (i < input_len && j + 1 < result_size) {
        reti = regexec(&regex, buf + i, 1, &match, 0);
        if (reti == 0 && match.rm_so == 0 && match.rm_eo > 0) {
            /* Possible weaknesses found:
             *  Condition 'j+1<result_size' is always true
             *  Condition 'j+1<result_size' is always true [knownConditionTrueFalse]
             */
            if (j > 0 && j + 1 < result_size) {
                result[j++] = ' ';
            }
            while (match.rm_so < match.rm_eo && j + 1 < result_size) {
                result[j++] = buf[i + (size_t)match.rm_so];
                match.rm_so++;
            }
            i += (size_t)match.rm_eo;
        } else {
            result[j++] = buf[i++];
        }
    }

    result[j] = '\0';
    regfree(&regex);
    free(buf);
    return result;
}

int main(void)
{
    char *result;
    const char *input = "HelloWorldThisIsATest";

    result = add_spaces(input, MAX_INPUT_LEN);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    return 0;
}