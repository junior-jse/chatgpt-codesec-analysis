#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *snake_to_camel(const char *snake_str) {
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
    size_t remaining = len;

    while (1) {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == REG_NOMATCH) {
            snprintf(camel_str + i, remaining + 1, "%s", cursor);
            break;
        }

        if (matches[0].rm_so == -1 || matches[1].rm_so == -1) {
            snprintf(camel_str + i, remaining + 1, "%s", cursor);
            break;
        }

        size_t copy_len = matches[0].rm_so;
        if (copy_len > remaining) {
            break;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(camel_str + i, cursor, copy_len);
        i += copy_len;
        remaining -= copy_len;

        if (remaining == 0) {
            break;
        }
        char upper = cursor[matches[1].rm_so] - 32;
        camel_str[i++] = upper;
        /* Possible weaknesses found:
         *  remaining is assigned
         */
        remaining--;

        cursor += matches[0].rm_eo;
        /* Possible weaknesses found:
         *  remaining is overwritten
         *  Variable 'remaining' is reassigned a value before the old one has been used. [redundantAssignment]
         */
        remaining = len - (cursor - snake_str);
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