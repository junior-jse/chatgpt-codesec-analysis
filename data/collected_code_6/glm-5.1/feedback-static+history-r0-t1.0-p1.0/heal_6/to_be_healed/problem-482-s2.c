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

        size_t start = match.rm_so;
        size_t end = match.rm_eo;
        size_t length = end - start;

        char *buffer = malloc(length + 1);
        if (buffer == NULL) {
            regfree(&regex);
            return;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
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

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    find_sequences(argv[1]);
    return EXIT_SUCCESS;
}