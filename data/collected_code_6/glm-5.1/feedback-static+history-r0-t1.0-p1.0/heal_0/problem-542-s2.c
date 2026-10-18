#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* replace_with_colon(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[ ,\\.]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    for (size_t i = 0; i < len; ++i) {
        char buf[2] = {str[i], '\0'};
        ret = regexec(&regex, buf, 0, NULL, 0);
        if (ret == 0) {
            result[i] = ':';
        } else if (ret == REG_NOMATCH) {
            result[i] = str[i];
        } else {
            result[i] = str[i];
        }
    }
    result[len] = '\0';

    regfree(&regex);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char* modified = replace_with_colon(argv[1]);
    if (modified == NULL) {
        fprintf(stderr, "Failed to process string\n");
        return 1;
    }

    printf("%s\n", modified);
    free(modified);

    return 0;
}