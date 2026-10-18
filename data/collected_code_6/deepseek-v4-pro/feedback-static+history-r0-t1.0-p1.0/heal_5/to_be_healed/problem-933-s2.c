/* Repair history (previous round):
 * - Flawfinder memcpy was removed from camel_to_snake() and now appears in safe_str_copy().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

static int is_valid_input(const char *input) {
    if (input == NULL) {
        return 0;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    if (len == 0 || len >= 4096) {
        return 0;
    }
    
    return 1;
}

static size_t safe_str_copy(char *dest, size_t dest_size, const char *src, size_t src_len) {
    if (dest == NULL || src == NULL || dest_size == 0 || src_len == 0) {
        return 0;
    }
    
    size_t copy_len = (src_len < dest_size) ? src_len : (dest_size - 1);
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest, src, copy_len);
    return copy_len;
}

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    if (input_len == 0) {
        char *empty = (char *)malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }
    
    if (input_len >= 4096) {
        return NULL;
    }
    
    regex_t regex;
    const char *pattern = "([a-z0-9])([A-Z])";
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }
    
    size_t max_output_len = input_len * 2 + 1;
    char *result = (char *)malloc(max_output_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }
    
    size_t src_pos = 0;
    size_t dst_pos = 0;
    int found = 0;
    regmatch_t matches[3];
    
    while (src_pos < input_len && regexec(&regex, input + src_pos, 3, matches, 0) == 0) {
        size_t match_start = (size_t)matches[0].rm_so;
        size_t match_end = (size_t)matches[0].rm_eo;
        size_t first_end = (size_t)matches[1].rm_eo;
        size_t second_start = (size_t)matches[2].rm_so;
        
        size_t prefix_len = match_start;
        size_t first_char_offset = first_end - 1;
        
        size_t needed = dst_pos + prefix_len + 3;
        if (needed > max_output_len) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        
        if (prefix_len > 0) {
            size_t copied = safe_str_copy(result + dst_pos, max_output_len - dst_pos, input + src_pos, prefix_len);
            dst_pos += copied;
        }
        
        if (dst_pos < max_output_len) {
            result[dst_pos++] = input[src_pos + first_char_offset];
        }
        if (dst_pos < max_output_len) {
            result[dst_pos++] = '_';
        }
        if (dst_pos < max_output_len) {
            result[dst_pos++] = (char)tolower((unsigned char)input[src_pos + second_start]);
        }
        
        src_pos += match_end;
        found = 1;
    }
    
    if (src_pos < input_len) {
        size_t remaining = input_len - src_pos;
        if (dst_pos + remaining + 1 > max_output_len) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        
        size_t copied = safe_str_copy(result + dst_pos, max_output_len - dst_pos, input + src_pos, remaining);
        dst_pos += copied;
    }
    
    if (dst_pos >= max_output_len) {
        free(result);
        regfree(&regex);
        return NULL;
    }
    
    result[dst_pos] = '\0';
    regfree(&regex);
    
    char *final_result = NULL;
    if (!found) {
        final_result = (char *)malloc(input_len + 1);
        if (final_result != NULL) {
            size_t copied = safe_str_copy(final_result, input_len + 1, input, input_len);
            if (copied < input_len) {
                free(final_result);
                final_result = NULL;
            } else {
                final_result[copied] = '\0';
            }
        }
        free(result);
    } else {
        final_result = (char *)realloc(result, dst_pos + 1);
        if (final_result == NULL) {
            free(result);
        }
    }
    
    return final_result;
}

int main(void) {
    const char *test_strings[] = {
        "camelCase",
        "snake_case",
        "myVariableName",
        "HTTPResponseCode",
        "test123Case",
        "already_snake",
        "A",
        "",
        "ABC",
        "convertJSONToXML"
    };
    
    size_t test_count = sizeof(test_strings) / sizeof(test_strings[0]);
    
    for (size_t i = 0; i < test_count; i++) {
        char *converted = camel_to_snake(test_strings[i]);
        if (converted != NULL) {
            printf("\"%s\" -> \"%s\"\n", test_strings[i], converted);
            free(converted);
        } else {
            printf("\"%s\" -> conversion failed\n", test_strings[i]);
        }
    }
    
    return 0;
}