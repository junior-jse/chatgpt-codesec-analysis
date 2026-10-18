#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT_SIZE 4096

char *add_spaces_before_capitals(const char *input) {
    regex_t regex;
    regmatch_t match[1];
    const char *cursor = input;
    size_t input_len;
    size_t output_size;
    char *output;
    size_t out_idx = 0;
    int first_word = 1;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    input_len = strnlen(input, MAX_INPUT_SIZE);
    if (input_len == MAX_INPUT_SIZE) {
        return NULL;
    }

    output_size = input_len * 2 + 1;

    output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    ret = regcomp(&regex, "[A-Z][a-zA-Z]*", REG_EXTENDED);
    if (ret != 0) {
        free(output);
        return NULL;
    }

    while (*cursor != '\0') {
        size_t prefix_len;
        size_t word_len;

        ret = regexec(&regex, cursor, 1, match, 0);
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        if (match[0].rm_so < 0 || match[0].rm_eo < match[0].rm_so) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        prefix_len = (size_t)match[0].rm_so;
        word_len = (size_t)(match[0].rm_eo - match[0].rm_so);

        if (out_idx >= output_size || prefix_len >= output_size - out_idx) {
            regfree(&regex);
            free(output);
            return NULL;
        }
        memcpy(output + out_idx, cursor, prefix_len);
        out_idx += prefix_len;

        if (!first_word && out_idx > 0 && output[out_idx - 1] != ' ') {
            if (out_idx + 1 >= output_size) {
                regfree(&regex);
                free(output);
                return NULL;
            }
            output[out_idx++] = ' ';
        }

        if (out_idx >= output_size || word_len >= output_size - out_idx) {
            regfree(&regex);
            free(output);
            return NULL;
        }
        memcpy(output + out_idx, cursor + match[0].rm_so, word_len);
        out_idx += word_len;

        cursor += match[0].rm_eo;
        first_word = 0;
    }

    {
        size_t remaining = input_len - (size_t)(cursor - input);

        if (out_idx >= output_size || remaining >= output_size - out_idx) {
            regfree(&regex);
            free(output);
            return NULL;
        }

        memcpy(output + out_idx, cursor, remaining);
        out_idx += remaining;
        output[out_idx] = '\0';
    }

    regfree(&regex);
    return output;
}

int main(void) {
    const char *input = "HelloWorldThisIsATest";
    char *result = add_spaces_before_capitals(input);

    if (result == NULL) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", input);
    printf("Modified: %s\n", result);

    free(result);
    return EXIT_SUCCESS;
}