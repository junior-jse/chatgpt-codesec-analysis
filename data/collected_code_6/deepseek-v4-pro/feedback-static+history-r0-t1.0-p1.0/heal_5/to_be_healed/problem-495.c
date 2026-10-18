#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void remove_lowercase_substrings(const char *input, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return;
    }

    regex_t regex;
    regmatch_t match;
    int ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    
    if (ret != 0) {
        output[0] = '\0';
        return;
    }

    const char *current_pos = input;
    size_t out_pos = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);

    while (*current_pos != '\0' && out_pos < output_size) {
        ret = regexec(&regex, current_pos, 1, &match, 0);
        
        if (ret == REG_NOMATCH) {
            size_t remaining_in = input_len - (current_pos - input);
            size_t space_left = output_size - out_pos - 1;
            size_t copy_len = remaining_in < space_left ? remaining_in : space_left;
            if (copy_len > 0 && out_pos + copy_len < output_size) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(output + out_pos, current_pos, copy_len);
                out_pos += copy_len;
            }
            break;
        } else if (ret == 0) {
            size_t match_start = match.rm_so;
            if (match_start > 0) {
                size_t space_left = output_size - out_pos - 1;
                size_t copy_len = match_start < space_left ? match_start : space_left;
                if (copy_len > 0 && out_pos + copy_len < output_size) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(output + out_pos, current_pos, copy_len);
                    out_pos += copy_len;
                }
            }
            current_pos += match.rm_eo;
        } else {
            break;
        }
    }

    if (out_pos < output_size) {
        output[out_pos] = '\0';
    } else {
        output[output_size - 1] = '\0';
    }
    regfree(&regex);
}

int main(void) {
    const char *test_str = "HelloWorld123abcDEFxyz456";
    size_t buffer_size = 256;
    char *result = (char *)malloc(buffer_size * sizeof(char));
    
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    remove_lowercase_substrings(test_str, result, buffer_size);
    printf("Original: %s\n", test_str);
    printf("Result: %s\n", result);
    
    free(result);
    return 0;
}