#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

char *snake_to_camel(const char *snake_str) {
    if (snake_str == NULL) {
        return NULL;
    }

    regex_t regex;
    regmatch_t matches[2];
    int ret;

    ret = regcomp(&regex, "_([a-z])", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t len = strnlen(snake_str, SIZE_MAX);
    char *camel_str = malloc(len + 1);
    if (camel_str == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t i = 0;
    const char *cursor = snake_str;

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == REG_NOMATCH || matches[0].rm_so == -1 || matches[1].rm_so == -1) {
            size_t remaining = strnlen(cursor, len - (cursor - snake_str));
            if (i + remaining < len + 1) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(camel_str + i, cursor, remaining);
                i += remaining;
            }
            break;
        }

        size_t copy_len = matches[0].rm_so;
        
        if (i + copy_len < len + 1) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(camel_str + i, cursor, copy_len);
            i += copy_len;
        }

        cursor += matches[0].rm_so;

        if (*cursor != '\0' && i < len + 1) {
            camel_str[i++] = toupper((unsigned char)cursor[matches[1].rm_so - matches[0].rm_so]);
            cursor += matches[0].rm_eo - matches[0].rm_so;
        }
    }

    camel_str[i] = '\0';
    regfree(&regex);
    return camel_str;
}

int main(void) {
    const char *input = "convert_this_snake_case_string";
    char *output = snake_to_camel(input);

    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Conversion failed\n");
        return 1;
    }

    return 0;
}