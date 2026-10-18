#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* minimize_length(const char* str) {
    if (str == NULL) {
        char* empty = (char*)malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    size_t len = strnlen(str, 100);

    if (len == 0) {
        char* empty = (char*)malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    if (len >= 100 && str[len] != '\0') {
        char* empty = (char*)malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    int freq[256] = {0};
    size_t i;

    for (i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    int max_freq = -1;
    char max_char = '\0';

    for (i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_char = (char)i;
        }
    }

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != max_char) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    char str[101];

    if (fgets(str, sizeof(str), stdin) == NULL) {
        return 1;
    }

    str[sizeof(str) - 1] = '\0';

    size_t len = strnlen(str, sizeof(str));

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }

    if (len >= 100) {
        return 1;
    }

    char* minimized = minimize_length(str);
    if (minimized == NULL) {
        return 1;
    }

    printf("%s\n", minimized);
    free(minimized);

    return 0;
}