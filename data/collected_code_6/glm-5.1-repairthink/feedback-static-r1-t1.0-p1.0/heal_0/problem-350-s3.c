#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* minimize_string_length(const char* str) {
    size_t len = strlen(str);
    if (len == 0) {
        char* empty = malloc(1);
        if (!empty) return NULL;
        empty[0] = '\0';
        return empty;
    }

    int freq[256] = {0};
    for (size_t i = 0; i < len; ++i) {
        freq[(unsigned char)str[i]]++;
    }

    int max_freq = 0;
    char target_char = str[0];
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            target_char = (char)i;
        }
    }

    size_t new_len = len - max_freq;
    char* result = malloc(new_len + 1);
    if (!result) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; ++i) {
        if (str[i] != target_char) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main() {
    const char* input = "abracadabra";
    char* output = minimize_string_length(input);
    
    if (output) {
        printf("Original: %s\n", input);
        printf("Minimized: %s\n", output);
        free(output);
    }

    return 0;
}