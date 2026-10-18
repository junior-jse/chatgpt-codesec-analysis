#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void safe_memcpy(char *dest, size_t dest_size, size_t dest_offset, 
                 const char *src, size_t src_len) {
    if (dest_offset >= dest_size) {
        return;
    }
    size_t available = dest_size - dest_offset;
    size_t copy_len = src_len > available ? available : src_len;
    if (copy_len > 0) {
        memcpy(dest + dest_offset, src, copy_len);
    }
}

void remove_lowercase_substrings(const char *input, char *output, size_t output_size) {
    regex_t regex;
    regmatch_t match;
    size_t out_pos = 0;
    const char *cursor = input;
    int result;

    if (output_size == 0) {
        return;
    }

    output[0] = '\0';

    result = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (result != 0) {
        return;
    }

    while (*cursor != '\0' && out_pos < output_size - 1) {
        result = regexec(&regex, cursor, 1, &match, 0);
        if (result == REG_NOMATCH) {
            size_t remaining = safe_strlen(cursor, output_size);
            if (remaining > output_size - out_pos - 1) {
                remaining = output_size - out_pos - 1;
            }
            safe_memcpy(output, output_size, out_pos, cursor, remaining);
            out_pos += remaining;
            break;
        } else if (result == 0) {
            if (match.rm_so > 0) {
                size_t copy_len = (size_t)match.rm_so;
                if (copy_len > output_size - out_pos - 1) {
                    copy_len = output_size - out_pos - 1;
                }
                safe_memcpy(output, output_size, out_pos, cursor, copy_len);
                out_pos += copy_len;
            }
            cursor += match.rm_eo;
        } else {
            size_t remaining = safe_strlen(cursor, output_size);
            if (remaining > output_size - out_pos - 1) {
                remaining = output_size - out_pos - 1;
            }
            safe_memcpy(output, output_size, out_pos, cursor, remaining);
            out_pos += remaining;
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

    for (size_t i = 0; i < num_tests; i++) {
        size_t input_len = safe_strlen(test_strings[i], sizeof(result_buffer));
        if (input_len >= sizeof(result_buffer)) {
            fprintf(stderr, "Input string too long, skipping.\n");
            continue;
        }
        remove_lowercase_substrings(test_strings[i], result_buffer, sizeof(result_buffer));
        printf("Input: '%s' -> Output: '%s'\n", test_strings[i], result_buffer);
    }

    return 0;
}