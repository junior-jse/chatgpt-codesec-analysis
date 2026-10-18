#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT_LEN 4096

char *snake_to_camel(const char *input)
{
    regex_t regex;
    regmatch_t matches[2];
    const char *cursor;
    char *output;
    size_t output_len;
    size_t input_len;
    size_t i;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len == MAX_INPUT_LEN) {
        return NULL;
    }

    output_len = input_len + 1;
    output = (char *)malloc(output_len);
    if (output == NULL) {
        return NULL;
    }

    ret = regcomp(&regex, "_([a-z])", REG_EXTENDED);
    if (ret != 0) {
        free(output);
        return NULL;
    }

    i = 0;
    cursor = input;

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == 0 && matches[1].rm_so >= 0) {
            size_t prefix_len = (size_t)matches[0].rm_so;
            size_t j;
            char upper_char;

            if (i >= output_len || prefix_len >= output_len - i) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            for (j = 0; j < prefix_len; j++) {
                output[i] = cursor[j];
                i++;
            }

            upper_char = cursor[matches[1].rm_so];
            if (upper_char >= 'a' && upper_char <= 'z') {
                upper_char = (char)(upper_char - 'a' + 'A');
            }

            if (i >= output_len - 1) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            output[i] = upper_char;
            i++;

            cursor += matches[0].rm_eo;
        } else {
            size_t remaining;
            size_t j;

            if ((size_t)(cursor - input) > input_len) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            remaining = input_len - (size_t)(cursor - input);

            if (i >= output_len || remaining >= output_len - i) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            for (j = 0; j < remaining; j++) {
                output[i] = cursor[j];
                i++;
            }
            break;
        }
    }

    output[i] = '\0';
    regfree(&regex);
    return output;
}

int main(void)
{
    const char *input = "hello_world_this_is_c";
    char *result = snake_to_camel(input);

    if (result == NULL) {
        return 1;
    }

    printf("%s\n", result);
    free(result);
    return 0;
}