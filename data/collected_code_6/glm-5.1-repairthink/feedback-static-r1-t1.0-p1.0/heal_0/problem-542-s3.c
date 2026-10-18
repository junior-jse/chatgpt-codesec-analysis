#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

char* replace_with_colon(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
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

    size_t bufsize = len + 1;
    char* result = malloc(bufsize);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    const char* p = str;
    size_t offset = 0;

    while (*p) {
        regmatch_t match;
        if (regexec(&regex, p, 1, &match, 0) != 0) {
            break;
        }

        size_t prefix_len = match.rm_so;
        if (bufsize <= offset + prefix_len + 2) {
            bufsize += (prefix_len + 2);
            char* new_result = realloc(result, bufsize);
            if (new_result == NULL) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = new_result;
        }

        memcpy(result + offset, p, prefix_len);
        offset += prefix_len;
        result[offset++] = ':';
        p += match.rm_eo;
    }

    size_t remaining = strlen(p);
    if (bufsize <= offset + remaining + 1) {
        bufsize += (remaining + 1);
        char* new_result = realloc(result, bufsize);
        if (new_result == NULL) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        result = new_result;
    }

    memcpy(result + offset, p, remaining + 1);

    regfree(&regex);
    return result;
}

int main(int argc, char* argv[]) {
    const char* input = "Hello world, this.is a test";
    
    char* output = replace_with_colon(input);
    if (output == NULL) {
        fprintf(stderr, "Regex replacement failed\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", output);

    free(output);
    return EXIT_SUCCESS;
}