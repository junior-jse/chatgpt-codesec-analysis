#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match;
    int ret;

    ret = regcomp(&regex, "[A-Z][a-z]+", REG_EXTENDED);
    if (ret != 0) {
        regfree(&regex);
        return;
    }

    const char *cursor = str;
    while (1) {
        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        }
        if (ret != 0) {
            regfree(&regex);
            return;
        }

        if (match.rm_so == -1) {
            break;
        }

        int start = match.rm_so;
        int end = match.rm_eo;
        int length = end - start;

        char *buffer = malloc(length + 1);
        if (buffer == NULL) {
            regfree(&regex);
            return;
        }

        memcpy(buffer, cursor + start, length);
        buffer[length] = '\0';

        printf("%s\n", buffer);
        free(buffer);

        if (end == 0) {
            break;
        }
        cursor += end;
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