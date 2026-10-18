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

    size_t result_len = 0;
    size_t i = 0;
    while (i < len) {
        if (isspace((unsigned char)str[i])) {
            result_len++;
            i++;
        } else {
            size_t start = i;
            while (i < len && !isspace((unsigned char)str[i])) {
                i++;
            }
            size_t word_len = i - start;
            if (word_len != (size_t)k) {
                result_len += word_len;
            }
        }
    }

    if (result_len + 1 > buf_size) {
        return NULL;
    }

    char* result = (char*)malloc(result_len + 1);
    if (result == NULL) {
        return NULL;
    }

    i = 0;
    size_t j = 0;
    while (i < len) {
        while (i < len && isspace((unsigned char)str[i])) {
            result[j++] = str[i++];
        }
        size_t start = i;
        while (i < len && !isspace((unsigned char)str[i])) {
            i++;
        }
        size_t word_len = i - start;
        if (word_len != (size_t)k) {
            for (size_t w = start; w < i; w++) {
                result[j++] = str[w];
            }
        }
    }
    result[j] = '\0';

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(str, result, result_len + 1);
    free(result);
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