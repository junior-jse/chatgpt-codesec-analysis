#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[A-Z][a-z]+";
    const char *cursor;
    int ret;
    size_t str_len;
    size_t cursor_offset;

    if (!str) {
        return;
    }

    str_len = strnlen(str, 65536);
    if (str_len == 65536 && str[65536] != '\0') {
        return;
    }

    cursor = str;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        size_t errbuf_size = regerror(ret, &regex, NULL, 0);
        char *errbuf = (char *)malloc(errbuf_size);
        if (errbuf) {
            regerror(ret, &regex, errbuf, errbuf_size);
            fprintf(stderr, "Regex compilation failed: %s\n", errbuf);
            free(errbuf);
        }
        return;
    }

    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        int start = match.rm_so;
        int end = match.rm_eo;
        size_t len;

        if (start < 0 || end < 0 || end <= start) {
            cursor++;
            continue;
        }

        len = (size_t)(end - start);

        cursor_offset = (size_t)(cursor - str);
        if (cursor_offset + (size_t)start + len > str_len) {
            break;
        }

        if (cursor_offset + (size_t)end > str_len) {
            break;
        }

        if (len > 1024) {
            cursor += end;
            if ((size_t)(cursor - str) >= str_len) {
                break;
            }
            continue;
        }

        char *seq = (char *)malloc(len + 1);
        if (!seq) {
            regfree(&regex);
            return;
        }

        memcpy(seq, cursor + start, len);
        seq[len] = '\0';
        printf("%s\n", seq);
        free(seq);

        cursor += end;
        if ((size_t)(cursor - str) >= str_len) {
            break;
        }
    }

    regfree(&regex);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    find_sequences(argv[1]);
    return EXIT_SUCCESS;
}