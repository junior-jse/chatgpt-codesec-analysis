#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void find_sequences(const char *str, size_t length) {
    regex_t regex;
    int ret;
    regmatch_t match;
    size_t offset = 0;

    ret = regcomp(&regex, "[A-Z][a-z]+", REG_EXTENDED);
    if (ret != 0) {
        return;
    }

    while (offset < length) {
        ret = regexec(&regex, str + offset, 1, &match, 0);
        if (ret == REG_NOMATCH || ret != 0) {
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

int main(int argc, const char * const argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    size_t len = strnlen(argv[1], 1048576);
    find_sequences(argv[1], len);

    return EXIT_SUCCESS;
}