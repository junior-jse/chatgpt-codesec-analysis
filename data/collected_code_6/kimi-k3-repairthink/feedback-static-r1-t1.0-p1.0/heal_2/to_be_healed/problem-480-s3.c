#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

char findMaxOccurringChar(const char *str, size_t strSize)
{
    if (str == NULL || strSize == 0) {
        return '\0';
    }

    size_t len = strnlen(str, strSize);
    if (len == 0) {
        return '\0';
    }

    int count[UCHAR_MAX + 1] = {0};
    size_t i;

    for (i = 0; i < len; i++) {
        count[(unsigned char)str[i]]++;
    }

    int maxCount = 0;
    char maxChar = '\0';
    int j;

    for (j = 0; j <= UCHAR_MAX; j++) {
        if (count[j] > maxCount) {
            maxCount = count[j];
            maxChar = (char)j;
        }
    }

    return maxChar;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str[MAX_INPUT_SIZE];

    printf("Enter a string: ");
    if (fgets(str, sizeof(str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strnlen(str, sizeof(str));

    if (len == sizeof(str)) {
        str[sizeof(str) - 1] = '\0';
        len = sizeof(str) - 1;
    }

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Empty string provided\n");
        return 1;
    }

    char maxChar = findMaxOccurringChar(str, sizeof(str));

    if (maxChar != '\0') {
        printf("Maximum occurring character: '%c'\n", maxChar);
    } else {
        printf("No characters found\n");
    }

    return 0;
}