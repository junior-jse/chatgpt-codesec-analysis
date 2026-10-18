#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate_min_length(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0) {
        return 0;
    }
    
    int freq[256] = {0};
    
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }
    
    int max_freq = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
        }
    }
    
    return (int)len - max_freq;
}

char* minimize_string(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    size_t len = strlen(str);
    if (len == 0) {
        char *empty = (char *)malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }
    
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }
    
    int max_freq = 0;
    int target_char = -1;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            target_char = i;
        }
    }
    
    size_t new_len = len - max_freq;
    char *result = (char *)malloc(new_len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if ((unsigned char)str[i] != target_char) {
            result[j++] = str[i];
        }
    }
    
    result[j] = '\0';
    return result;
}

int main() {
    const char *input = "abracadabra";
    
    int min_len = calculate_min_length(input);
    char *minimized = minimize_string(input);
    
    if (minimized != NULL) {
        printf("Original: %s\n", input);
        printf("Minimized: %s\n", minimized);
        printf("Min Length: %d\n", min_len);
        free(minimized);
    }
    
    return 0;
}