#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *minimize_string(const char *str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        char *result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }

    if (len == 0) {
        char *result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t freq[256] = {0};
    
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    size_t min_freq = len;
    char char_to_remove = str[0];
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (freq[c] < min_freq) {
            min_freq = freq[c];
            char_to_remove = str[i];
        }
    }

    char *result = malloc(len - min_freq + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != char_to_remove) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char *input = "aabbcc";
    size_t input_len = strlen(input);
    char *result = minimize_string(input, input_len + 1);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }
    return 0;
}