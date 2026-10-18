#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

char *camel_to_snake(const char *camel_str) {
    if (camel_str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "([a-z0-9])([A-Z])", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t in_len = strnlen(camel_str, SIZE_MAX);
    if (in_len > SIZE_MAX / 2) {
        regfree(&regex);
        return NULL;
    }

    size_t max_len = in_len * 2 + 1;
    char *temp1 = malloc(max_len);
    char *temp2 = malloc(max_len);
    if (temp1 == NULL || temp2 == NULL) {
        free(temp1);
        free(temp2);
        regfree(&regex);
        return NULL;
    }

    memcpy(temp1, camel_str, in_len);
    temp1[in_len] = '\0';

    regmatch_t matches[3];
    size_t offset = 0;
    size_t current_len = in_len;

    while (offset < current_len) {
        if (regexec(&regex, temp1 + offset, 3, matches, 0) == 0) {
            size_t prefix_len = offset + matches[2].rm_so;
            size_t match_end_offset = offset + matches[2].rm_eo;
            size_t suffix_len = current_len - match_end_offset;

            memcpy(temp2, temp1, prefix_len);
            temp2[prefix_len] = '_';
            temp2[prefix_len + 1] = temp1[prefix_len] + 32;
            memcpy(temp2 + prefix_len + 2, temp1 + prefix_len + 1, suffix_len);

            current_len = prefix_len + 2 + suffix_len;
            temp2[current_len] = '\0';

            memcpy(temp1, temp2, current_len);
            temp1[current_len] = '\0';

            offset = prefix_len + 2;
        } else {
            break;
        }
    }

    regfree(&regex);

    char *final_str = malloc(current_len + 1);
    if (final_str == NULL) {
        free(temp1);
        free(temp2);
        return NULL;
    }

    memcpy(final_str, temp1, current_len);
    final_str[current_len] = '\0';

    free(temp1);
    free(temp2);

    return final_str;
}

int main(int argc, char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camel_case_string>\n", argv[0]);
        return 1;
    }

    char *snake_str = camel_to_snake(argv[1]);
    if (snake_str == NULL) {
        fprintf(stderr, "Conversion failed\n");
        return 1;
    }

    printf("%s\n", snake_str);

    free(snake_str);
    return 0;
}