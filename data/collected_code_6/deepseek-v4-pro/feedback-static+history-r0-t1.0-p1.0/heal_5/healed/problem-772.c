#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_of_length(char* str, int k) {
    if (str == NULL || k <= 0) {
        return str;
    }

    size_t max_len = strlen(str);

    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    
    while (i < max_len) {
        while (i < max_len && isspace((unsigned char)str[i])) {
            result[j++] = str[i];
            i++;
        }

        size_t start = i;
        while (i < max_len && !isspace((unsigned char)str[i])) {
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

    size_t result_len = j;
    if (result_len > max_len) {
        free(result);
        return NULL;
    }
    
    memmove(str, result, result_len + 1);
    
    free(result);
    return str;
}

int main(void) {
    char* str = NULL;
    size_t buf_size = 256;
    int k;

    str = (char*)malloc(buf_size);
    if (str == NULL) {
        return 1;
    }

    printf("Enter a string: ");
    if (fgets(str, buf_size, stdin) == NULL) {
        free(str);
        return 1;
    }
    
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }

    if (len > 0 && str[len - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        free(str);
        return 1;
    }

    remove_words_of_length(str, k);
    printf("Result: \"%s\"\n", str);

    free(str);
    return 0;
}