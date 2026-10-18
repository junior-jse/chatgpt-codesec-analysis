#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include <ctype.h>

static char *safe_str_append(char *dest, size_t *dest_len, size_t *dest_capacity, 
                              const char *src, size_t src_len) {
    if (*dest_len + src_len >= *dest_capacity) {
        size_t new_capacity = *dest_len + src_len + 32;
        char *new_dest = (char *)realloc(dest, new_capacity);
        if (new_dest == NULL) {
            free(dest);
            return NULL;
        }
        dest = new_dest;
        *dest_capacity = new_capacity;
    }
    if (*dest_len + src_len < *dest_capacity) {
        memcpy(dest + *dest_len, src, src_len);
        *dest_len += src_len;
        dest[*dest_len] = '\0';
    }
    return dest;
}

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    const char *pattern = "([A-Z])";
    regex_t regex;
    regmatch_t match;
    char *result = NULL;
    char *current = NULL;

    size_t input_len = strlen(input);
    if (input_len == 0) {
        result = (char *)malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t capacity = input_len * 2 + 2;
    size_t length = 0;
    size_t pos = 0;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return NULL;
    }

    result = (char *)malloc(capacity);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }
    result[0] = '\0';

    while (pos < input_len) {
        if (regexec(&regex, input + pos, 1, &match, 0) == 0) {
            size_t match_pos = match.rm_so;
            size_t match_len = match.rm_eo - match.rm_so;

            if (match_pos > 0) {
                result = safe_str_append(result, &length, &capacity, 
                                         input + pos, match_pos);
                if (result == NULL) {
                    regfree(&regex);
                    return NULL;
                }
            }

            if (length > 0 && result[length - 1] != '_') {
                result = safe_str_append(result, &length, &capacity, "_", 1);
                if (result == NULL) {
                    regfree(&regex);
                    return NULL;
                }
            }

            current = (char *)malloc(match_len + 1);
            if (current == NULL) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            memcpy(current, input + pos + match_pos, match_len);
            current[match_len] = '\0';

            for (size_t i = 0; i < match_len; i++) {
                current[i] = (char)tolower((unsigned char)current[i]);
            }

            result = safe_str_append(result, &length, &capacity, 
                                     current, match_len);
            free(current);
            
            if (result == NULL) {
                regfree(&regex);
                return NULL;
            }

            pos += match_pos + match_len;
        } else {
            size_t remaining = input_len - pos;
            result = safe_str_append(result, &length, &capacity, 
                                     input + pos, remaining);
            if (result == NULL) {
                regfree(&regex);
                return NULL;
            }
            break;
        }
    }

    regfree(&regex);
    return result;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    const char *test_strings[] = {
        "camelCaseString",
        "thisIsATest",
        "single",
        "",
        "ALLCAPS",
        "mixedCASEString"
    };
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char *converted = camel_to_snake(test_strings[i]);
        if (converted != NULL) {
            printf("%s -> %s\n", test_strings[i], converted);
            free(converted);
        } else {
            printf("%s -> (conversion failed)\n", test_strings[i]);
        }
    }

    return 0;
}