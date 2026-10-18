#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_of_length(char* str, int k) {
    if (str == NULL || k <= 0) {
        return str;
    }

    size_t max_len = 0;
    size_t pos = 0;
    while (str[pos] != '\0') {
        max_len++;
        pos++;
        if (pos > 65536) break;
    }

    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    
    while (i < max_len && str[i] != '\0') {
        while (i < max_len && str[i] != '\0' && isspace((unsigned char)str[i])) {
            if (j < max_len) {
                result[j++] = str[i];
            }
            i++;
        }

        size_t start = i;
        while (i < max_len && str[i] != '\0' && !isspace((unsigned char)str[i])) {
            i++;
        }
        size_t word_len = i - start;

        if (word_len != (size_t)k) {
            size_t w;
            for (w = start; w < i; w++) {
                if (j < max_len) {
                    result[j++] = str[w];
                }
            }
        }
    }

    result[j] = '\0';
    
    size_t result_len = j;
    size_t original_len = 0;
    size_t p = 0;

    while (p < max_len && str[p] != '\0') {
        original_len++;
        p++;
    }

    if (result_len <= original_len) {
        size_t idx;
        for (idx = 0; idx <= result_len && idx < max_len + 1; idx++) {
            str[idx] = result[idx];
        }
    } else {
        size_t idx;
        for (idx = 0; idx < original_len + 1 && idx < max_len + 1; idx++) {
            str[idx] = result[idx];
        }
    }
    
    free(result);
    return str;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str[256];
    int k;

    printf("Enter a string: ");
    if (fgets(str, sizeof(str), stdin) == NULL) {
        return 1;
    }
    
    size_t len = 0;
    while (len < sizeof(str) && str[len] != '\0') {
        if (str[len] == '\n') {
            str[len] = '\0';
            break;
        }
        len++;
    }

    if (len >= sizeof(str)) {
        str[sizeof(str) - 1] = '\0';
    }

    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        return 1;
    }

    remove_words_of_length(str, k);
    printf("Result: \"%s\"\n", str);

    return 0;
}