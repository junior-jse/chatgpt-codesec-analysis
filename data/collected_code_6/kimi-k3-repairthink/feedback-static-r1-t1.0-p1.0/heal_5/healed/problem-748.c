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

    buf_size = input_len + 1;
    buf = malloc(buf_size);
    if (buf == NULL) {
        return NULL;
    }
    if (input_len >= buf_size) {
        free(buf);
        return NULL;
    }
    memcpy(buf, input, input_len);
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

    while (i < input_len) {
        reti = regexec(&regex, buf + i, 1, &match, 0);
        if (reti == 0 && match.rm_so == 0 && match.rm_eo > 0) {
            size_t word_len = (size_t)(match.rm_eo - match.rm_so);
            if (j + word_len + 1 >= result_size) {
                free(result);
                regfree(&regex);
                free(buf);
                return NULL;
            }
            if (j > 0) {
                result[j++] = ' ';
            }
            while (match.rm_so < match.rm_eo) {
                result[j++] = buf[i + (size_t)match.rm_so];
                match.rm_so++;
            }
            i += (size_t)match.rm_eo;
        } else {
            if (j + 1 >= result_size) {
                free(result);
                regfree(&regex);
                free(buf);
                return NULL;
            }
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