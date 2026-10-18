#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[A-Z][a-z]+";
    const char *cursor = str;
    int ret;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char error_buffer[256];
    size_t error_buffer_size = sizeof(error_buffer);
    size_t str_len;

    if (str == NULL) {
        fprintf(stderr, "Input string is NULL\n");
        return;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    str_len = strlen(str);
    if (str_len == 0) {
        return;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        regerror(ret, &regex, error_buffer, error_buffer_size);
        error_buffer[error_buffer_size - 1] = '\0';
        fprintf(stderr, "Could not compile regex: %s\n", error_buffer);
        return;
    }

    while (cursor < str + str_len && *cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            regerror(ret, &regex, error_buffer, error_buffer_size);
            error_buffer[error_buffer_size - 1] = '\0';
            fprintf(stderr, "Regex match failed: %s\n", error_buffer);
            break;
        }

        if (match.rm_eo > match.rm_so && match.rm_eo <= (regoff_t)(str + str_len - cursor)) {
            printf("Found sequence: %.*s\n", (int)(match.rm_eo - match.rm_so), cursor + match.rm_so);
            cursor += match.rm_eo;
        } else {
            break;
        }
    }

    regfree(&regex);
}

int main(void) {
    const char *test_string = "Hello World This Is A Test String With Sequences Like Abcdef And Xyz";
    find_sequences(test_string);
    return 0;
}