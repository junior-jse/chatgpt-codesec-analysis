#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int remove_lowercase_substrings(const char *input, size_t input_len, char **output)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    char *result;
    size_t result_len;
    size_t remaining_len;
    int ret;

    if (input == NULL || output == NULL) {
        return -1;
    }

    *output = NULL;

    result = malloc(input_len + 1);
    if (result == NULL) {
        return -1;
    }

    ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return -1;
    }

    cursor = input;
    remaining_len = input_len;
    result_len = 0;

    while (remaining_len > 0) {
        ret = regexec(&regex, cursor, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            if (result_len + remaining_len > input_len) {
                regfree(&regex);
                free(result);
                return -1;
            }
            memcpy(result + result_len, cursor, remaining_len);
            result_len += remaining_len;
            break;
        }

        if (ret != 0) {
            regfree(&regex);
            free(result);
            return -1;
        }

        if ((size_t)match.rm_so > remaining_len || (size_t)match.rm_eo > remaining_len) {
            regfree(&regex);
            free(result);
            return -1;
        }

        if (result_len + (size_t)match.rm_so > input_len) {
            regfree(&regex);
            free(result);
            return -1;
        }

        memcpy(result + result_len, cursor, (size_t)match.rm_so);
        result_len += (size_t)match.rm_so;
        cursor += match.rm_eo;
        remaining_len -= (size_t)match.rm_eo;
    }

    result[result_len] = '\0';
    regfree(&regex);
    *output = result;

    return 0;
}

int main(void)
{
    const char input[] = "HelloWorldABCdefGHIjklMNOP";
    size_t input_len = sizeof(input) - 1;
    char *output = NULL;

    if (remove_lowercase_substrings(input, input_len, &output) != 0) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Input:  %s\n", input);
    printf("Output: %s\n", output);

    free(output);

    return EXIT_SUCCESS;
}