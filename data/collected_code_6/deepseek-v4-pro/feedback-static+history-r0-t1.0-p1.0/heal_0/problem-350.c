#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* minimize_string(const char* str) {
    int count[256] = {0};
    int len = strlen(str);
    int max_freq = 0;
    char max_char = '\0';
    int i, j;

    if (len == 0) {
        char* empty = (char*)malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    for (i = 0; i < len; i++) {
        count[(unsigned char)str[i]]++;
    }

    for (i = 0; i < 256; i++) {
        if (count[i] > max_freq) {
            max_freq = count[i];
            max_char = (char)i;
        }
    }

    char* result = (char*)malloc((len - max_freq + 1) * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    j = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != max_char) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char* test1 = "aabbbcc";
    const char* test2 = "hello world";
    const char* test3 = "abc";
    const char* test4 = "";
    const char* test5 = "aaaa";
    char* result;

    result = minimize_string(test1);
    if (result != NULL) {
        printf("%s -> %s\n", test1, result);
        free(result);
    }

    result = minimize_string(test2);
    if (result != NULL) {
        printf("%s -> %s\n", test2, result);
        free(result);
    }

    result = minimize_string(test3);
    if (result != NULL) {
        printf("%s -> %s\n", test3, result);
        free(result);
    }

    result = minimize_string(test4);
    if (result != NULL) {
        printf("%s -> %s\n", test4, result);
        free(result);
    }

    result = minimize_string(test5);
    if (result != NULL) {
        printf("%s -> %s\n", test5, result);
        free(result);
    }

    return 0;
}