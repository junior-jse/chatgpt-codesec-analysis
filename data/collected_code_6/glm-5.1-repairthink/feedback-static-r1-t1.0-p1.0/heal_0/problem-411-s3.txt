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

    size_t len = strlen(snake_str);
    char *camel_str = malloc(len + 1);
    if (camel_str == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t i = 0;
    const char *cursor = snake_str;

    while (1) {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == REG_NOMATCH) {
            strcpy(camel_str + i, cursor);
            break;
        }

        if (matches[0].rm_so == -1 || matches[1].rm_so == -1) {
            strcpy(camel_str + i, cursor);
            break;
        }

        size_t copy_len = matches[0].rm_so;
        memcpy(camel_str + i, cursor, copy_len);
        i += copy_len;

        char upper = cursor[matches[1].rm_so] - 32;
        camel_str[i++] = upper;

        cursor += matches[0].rm_eo;
    }

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