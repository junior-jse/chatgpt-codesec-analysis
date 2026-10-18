#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STR_LEN 8192

char* replace_with_colon(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[ ,\\.]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t len = strnlen(str, MAX_STR_LEN);
    if (len == MAX_STR_LEN) {
        regfree(&regex);
        return NULL;
    }

    size_t buffer_size = len + 1;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    regmatch_t match;
    size_t src_offset = 0;
    size_t dest_offset = 0;

    while (src_offset < len) {
        ret = regexec(&regex, str + src_offset, 1, &match, 0);
        if (ret == 0) {
            size_t match_start = src_offset + match.rm_so;
            size_t match_len = match.rm_eo - match.rm_so;
            size_t prefix_len = match_start - src_offset;
            
            for (size_t i = 0; i < prefix_len && dest_offset < buffer_size - 1; i++) {
                result[dest_offset++] = str[src_offset + i];
            }

            for (size_t i = 0; i < match_len && dest_offset < buffer_size - 1; i++) {
                result[dest_offset++] = ':';
            }

            src_offset = src_offset + match.rm_eo;
        } else {
            size_t remaining = len - src_offset;
            
            for (size_t i = 0; i < remaining && dest_offset < buffer_size - 1; i++) {
                result[dest_offset++] = str[src_offset + i];
            }
            
            src_offset += remaining;
        }
    }

    result[dest_offset] = '\0';
    regfree(&regex);

    return result;
}

int main(void) {
    const char* original = "Hello world, this.is a test.";
    char* replaced = replace_with_colon(original);

    if (replaced != NULL) {
        printf("%s\n", replaced);
        free(replaced);
    }

    return 0;
}