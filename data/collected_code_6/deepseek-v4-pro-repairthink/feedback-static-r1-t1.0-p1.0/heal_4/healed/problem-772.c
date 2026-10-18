#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_of_length(char* str, size_t buf_size, int k) {
    if (str == NULL || k <= 0 || buf_size == 0) {
        return str;
    }

    size_t len = 0;
    while (len < buf_size && str[len] != '\0') {
        len++;
    }
    if (len == buf_size) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < len) {
        if (isspace((unsigned char)str[i])) {
            str[j++] = str[i++];
        } else {
            size_t start = i;
            while (i < len && !isspace((unsigned char)str[i])) {
                i++;
            }
            size_t word_len = i - start;
            if (word_len != (size_t)k) {
                while (start < i) {
                    str[j++] = str[start++];
                }
            }
        }
    }
    str[j] = '\0';
    return str;
}

int main(void) {
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    printf("Enter a string: ");
    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        free(line);
        return 1;
    }
    if (nread > 0 && line[nread - 1] == '\n') {
        line[nread - 1] = '\0';
    }

    int k;
    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        free(line);
        return 1;
    }

    if (remove_words_of_length(line, len, k) == NULL) {
        free(line);
        return 1;
    }
    printf("Result: \"%s\"\n", line);

    free(line);
    return 0;
}