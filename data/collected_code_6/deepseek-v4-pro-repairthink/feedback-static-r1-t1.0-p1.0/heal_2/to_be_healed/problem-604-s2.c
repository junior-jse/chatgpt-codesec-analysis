#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse_words(char *str) {
    if (str == NULL) return;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len == 0) return;
    char *result = malloc(len + 1);
    if (result == NULL) return;
    size_t result_idx = 0;
    int i = (int)len - 1;
    while (i >= 0) {
        while (i >= 0 && str[i] == ' ') i--;
        if (i < 0) break;
        int word_end = i + 1;
        while (i >= 0 && str[i] != ' ') i--;
        int word_start = i + 1;
        if (result_idx > 0) result[result_idx++] = ' ';
        size_t word_len = (size_t)(word_end - word_start);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_idx, str + word_start, word_len);
        result_idx += word_len;
    }
    result[result_idx] = '\0';
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(str, result, result_idx + 1);
    free(result);
}

int main(void) {
    char str1[] = "Hello World";
    reverse_words(str1);
    printf("%s\n", str1);

    char str2[] = "This is a test";
    reverse_words(str2);
    printf("%s\n", str2);

    char str3[] = "Single";
    reverse_words(str3);
    printf("%s\n", str3);

    char str4[] = "";
    reverse_words(str4);
    printf("%s\n", str4);

    char str5[] = "   Multiple   spaces   ";
    reverse_words(str5);
    printf("%s\n", str5);

    return 0;
}