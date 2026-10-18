#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

char* replace_with_colon(const char* str, size_t len) {
    if (str == NULL) {
        return NULL;
    }

    if (len == 0) {
        char* empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[ ,\\.]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    char* result = malloc(len + 1);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    const char* p = str;
    size_t offset = 0;

    while (*p && (size_t)(p - str) < len) {
        regmatch_t match;
        if (regexec(&regex, p, 1, &match, 0) != 0) {
            break;
        }

        size_t prefix_len = match.rm_so;
        size_t copy_len = prefix_len;
        if (offset + copy_len > len) {
            copy_len = len - offset;
        }
        
        for (size_t i = 0; i < copy_len; ++i) {
            result[offset + i] = p[i];
        }
        offset += copy_len;
        
        if (offset < len) {
            result[offset++] = ':';
        }
        
        p += match.rm_eo;
    }

    size_t pos = (size_t)(p - str);
    size_t max_remaining = (pos < len) ? (len - pos) : 0;
    size_t remaining = strnlen(p, max_remaining);
    
    if (offset + remaining > len) {
        remaining = len - offset;
    }
    
    for (size_t i = 0; i < remaining; ++i) {
        result[offset + i] = p[i];
    }
    offset += remaining;
    
    result[offset] = '\0';

    regfree(&regex);
    return result;
}

int main(int argc, char* argv[]) {
    const char* input = "Hello world, this.is a test";
    size_t len = sizeof("Hello world, this.is a test") - 1;
    
    char* output = replace_with_colon(input, len);
    if (output == NULL) {
        fprintf(stderr, "Regex replacement failed\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", output);

    free(output);
    return EXIT_SUCCESS;
}