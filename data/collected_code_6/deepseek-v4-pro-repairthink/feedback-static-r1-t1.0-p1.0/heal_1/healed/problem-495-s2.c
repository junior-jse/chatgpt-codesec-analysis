#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void remove_lowercase_substrings(const char *input, char *output, size_t output_size) {
    regex_t regex;
    size_t out_pos = 0;
    int result;
    const char *cursor;
    regmatch_t match;

    if (output_size == 0) return;
    output[0] = '\0';

    result = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (result != 0) return;

    cursor = input;
    while (*cursor != '\0' && out_pos < output_size - 1) {
        result = regexec(&regex, cursor, 1, &match, 0);
        if (result == REG_NOMATCH) {
            size_t i = 0;
            while (cursor[i] != '\0' && out_pos < output_size - 1) {
                output[out_pos++] = cursor[i++];
            }
            break;
        } else if (result == 0) {
            size_t i;
            size_t prefix = (size_t)match.rm_so;
            for (i = 0; i < prefix && cursor[i] != '\0' && out_pos < output_size - 1; i++) {
                output[out_pos++] = cursor[i];
            }
            cursor += match.rm_eo;
        } else {
            size_t i = 0;
            while (cursor[i] != '\0' && out_pos < output_size - 1) {
                output[out_pos++] = cursor[i++];
            }
            break;
        }
    }

    output[out_pos] = '\0';
    regfree(&regex);
}

int main(void) {
    const char *test_strings[] = {
        "HelloWorld",
        "abcXYZdef",
        "ABC123low456",
        "test",
        "UPPERlowerUPPER",
        "A1b2C3d4",
        "",
        "alllowercase"
    };
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    char result_buffer[256];
    size_t i;

    for (i = 0; i < num_tests; i++) {
        remove_lowercase_substrings(test_strings[i], result_buffer, sizeof(result_buffer));
        printf("Input: '%s' -> Output: '%s'\n", test_strings[i], result_buffer);
    }

    return 0;
}