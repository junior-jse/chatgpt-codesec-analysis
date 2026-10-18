#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void remove_lowercase_substrings(const char *input, char *output, size_t output_size) {
    regex_t regex;
    regmatch_t match;
    int offset = 0;
    size_t out_pos = 0;
    const char *cursor = input;
    int result;

    result = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (result != 0) {
        output[0] = '\0';
        return;
    }

    while (*cursor != '\0' && out_pos < output_size - 1) {
        result = regexec(&regex, cursor, 1, &match, 0);
        if (result == REG_NOMATCH) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t remaining = strlen(cursor);
            if (out_pos + remaining >= output_size) {
                remaining = output_size - out_pos - 1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + out_pos, cursor, remaining);
            out_pos += remaining;
            break;
        } else if (result == 0) {
            if (match.rm_so > 0) {
                size_t copy_len = match.rm_so;
                if (out_pos + copy_len >= output_size) {
                    copy_len = output_size - out_pos - 1;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(output + out_pos, cursor, copy_len);
                out_pos += copy_len;
            }
            cursor += match.rm_eo;
        } else {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t remaining = strlen(cursor);
            if (out_pos + remaining >= output_size) {
                remaining = output_size - out_pos - 1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + out_pos, cursor, remaining);
            out_pos += remaining;
            break;
        }
    }

    output[out_pos] = '\0';
    regfree(&regex);
    (void)offset;
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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result_buffer[256];

    for (size_t i = 0; i < num_tests; i++) {
        remove_lowercase_substrings(test_strings[i], result_buffer, sizeof(result_buffer));
        printf("Input: '%s' -> Output: '%s'\n", test_strings[i], result_buffer);
    }

    return 0;
}