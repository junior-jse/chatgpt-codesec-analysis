#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT_LENGTH 4096

int remove_lowercase_substrings(const char *input, char **output)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    char *result;
    size_t result_len;
    size_t input_len;
    size_t copy_len;
    size_t capacity;
    int ret;

    if (input == NULL || output == NULL) {
        return -1;
    }

    *output = NULL;

    input_len = strnlen(input, MAX_INPUT_LENGTH);
    if (input_len >= MAX_INPUT_LENGTH) {
        return -1;
    }

    capacity = input_len + 1;

    result = malloc(capacity);
    if (result == NULL) {
        return -1;
    }

    ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return -1;
    }

    cursor = input;
    result_len = 0;

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            copy_len = strlen(cursor);
            if (result_len >= capacity || copy_len > (capacity - result_len - 1)) {
                regfree(&regex);
                free(result);
                return -1;
            }
            memcpy(result + result_len, cursor, copy_len);
            result_len += copy_len;
            break;
        }

        if (ret != 0) {
            regfree(&regex);
            free(result);
            return -1;
        }

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            free(result);
            return -1;
        }

        copy_len = (size_t)match.rm_so;
        if (result_len >= capacity || copy_len > (capacity - result_len - 1)) {
            regfree(&regex);
            free(result);
            return -1;
        }
        memcpy(result + result_len, cursor, copy_len);
        result_len += copy_len;
        cursor += match.rm_eo;
    }

    result[result_len] = '\0';
    regfree(&regex);
    *output = result;

    return 0;
}

int main(void)
{
    const char *input = "HelloWorldABCdefGHIjklMNOP";
    char *output = NULL;

    if (remove_lowercase_substrings(input, &output) != 0) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Input:  %s\n", input);
    printf("Output: %s\n", output);

    free(output);

    return EXIT_SUCCESS;
}