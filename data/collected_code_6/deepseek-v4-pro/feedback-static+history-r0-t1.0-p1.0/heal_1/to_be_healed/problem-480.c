#include <stdio.h>
#include <string.h>
#include <limits.h>

char find_max_occurring_char(const char *str) {
    int count[UCHAR_MAX + 1] = {0};
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    int len = strlen(str);
    char max_char = '\0';
    int max_count = 0;
    int i;

    for (i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)str[i];
        count[ch]++;
    }

    for (i = 0; i <= UCHAR_MAX; i++) {
        if (count[i] > max_count) {
            max_count = count[i];
            max_char = (char)i;
        }
    }

    return max_char;
}

int main(void) {
    /* Possible weaknesses found:
     *  Variable 'str' can be declared as const array [constVariable]
     */
    char str[] = "sample string with some characters";
    char result;

    result = find_max_occurring_char(str);
    printf("Max occurring character: %c\n", result);

    return 0;
}