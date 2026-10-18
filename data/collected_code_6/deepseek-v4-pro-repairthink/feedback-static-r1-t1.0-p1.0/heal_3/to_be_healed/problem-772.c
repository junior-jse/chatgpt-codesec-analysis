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

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
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
            size_t w;
            for (w = start; w < i; w++) {
                result[j++] = str[w];
            }
        }
    }

    result[j] = '\0';

    if (j + 1 > buf_size) {
        free(result);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(str, result, j + 1);
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
    str[strcspn(str, "\n")] = '\0';

    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        return 1;
    }

    if (remove_words_of_length(str, sizeof(str), k) == NULL) {
        return 1;
    }
    printf("Result: \"%s\"\n", str);

    return 0;
}