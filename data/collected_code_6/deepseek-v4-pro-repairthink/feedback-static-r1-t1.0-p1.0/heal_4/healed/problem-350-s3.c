#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_line(size_t *out_len) {
    size_t cap = 128;
    size_t total_len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;

    while (1) {
        if (fgets(buf + total_len, cap - total_len, stdin) == NULL) {
            if (total_len == 0) {
                free(buf);
                return NULL;
            }
            break;
        }

        size_t chunk_len = 0;
        while (total_len + chunk_len < cap && buf[total_len + chunk_len] != '\0') {
            chunk_len++;
        }
        total_len += chunk_len;

        if (total_len > 0 && buf[total_len - 1] == '\n') {
            buf[--total_len] = '\0';
            break;
        }

        if (total_len + 1 >= cap) {
            size_t newcap = cap * 2;
            char *newbuf = realloc(buf, newcap);
            if (!newbuf) {
                free(buf);
                return NULL;
            }
            buf = newbuf;
            cap = newcap;
        }
    }
    *out_len = total_len;
    return buf;
}

char* minimize_length(char* str, size_t len) {
    if (str == NULL || len == 0) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    int max_freq = -1;
    char max_char = '\0';
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_char = (char)i;
        }
    }

    char* result = malloc(len + 1);
    if (!result) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != max_char) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    size_t len;
    char* line = read_line(&len);
    if (!line) return 1;

    char* minimized = minimize_length(line, len);
    if (!minimized) {
        free(line);
        return 1;
    }

    printf("%s\n", minimized);
    free(minimized);
    free(line);
    return 0;
}