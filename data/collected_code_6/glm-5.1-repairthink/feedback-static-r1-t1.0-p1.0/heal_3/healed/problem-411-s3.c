#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

char *snake_to_camel(const char *snake_str, size_t len) {
    if (snake_str == NULL) {
        return NULL;
    }

    char *safe_str = malloc(len + 1);
    if (safe_str == NULL) {
        return NULL;
    }
    memcpy(safe_str, snake_str, len);
    safe_str[len] = '\0';

    regex_t regex;
    regmatch_t matches[2];
    int ret;

    ret = regcomp(&regex, "_([a-z])", REG_EXTENDED);
    if (ret != 0) {
        free(safe_str);
        return NULL;
    }

    char *camel_str = malloc(len + 1);
    if (camel_str == NULL) {
        regfree(&regex);
        free(safe_str);
        return NULL;
    }

    size_t i = 0;
    const char *cursor = safe_str;

    while (1) {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == REG_NOMATCH || matches[0].rm_so == -1 || matches[1].rm_so == -1) {
            size_t remaining = (len + 1) - i;
            snprintf(camel_str + i, remaining, "%s", cursor);
            break;
        }

        size_t copy_len = matches[0].rm_so;
        size_t remaining = (len + 1) - i;
        
        if (copy_len > remaining) {
            copy_len = remaining;
        }

        memcpy(camel_str + i, cursor, copy_len);
        i += copy_len;

        remaining = (len + 1) - i;
        if (remaining > 0) {
            camel_str[i++] = toupper((unsigned char)cursor[matches[1].rm_so]);
        }

        cursor += matches[0].rm_eo;
    }

    regfree(&regex);
    free(safe_str);
    return camel_str;
}

int main(void) {
    const char *input = "convert_this_snake_case_string";
    char *output = snake_to_camel(input, strlen(input));

    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Conversion failed\n");
        return 1;
    }

    return 0;
}