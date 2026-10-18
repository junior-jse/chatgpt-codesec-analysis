#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void find_sequences(const char *str) {
    regex_t regex;
    int ret;
    regmatch_t match;
    size_t offset = 0;
    size_t length = strlen(str);

    ret = regcomp(&regex, "[A-Z][a-z]+", REG_EXTENDED);
    if (ret != 0) {
        regfree(&regex);
        return;
    }

    while (offset < length) {
        ret = regexec(&regex, str + offset, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        }
        if (ret != 0) {
            break;
        }

        if (match.rm_so != -1 && match.rm_eo != -1) {
            printf("%.*s\n", match.rm_eo - match.rm_so, str + offset + match.rm_so);
            offset += match.rm_eo;
        } else {
            break;
        }
    }

    regfree(&regex);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    find_sequences(argv[1]);

    return EXIT_SUCCESS;
}