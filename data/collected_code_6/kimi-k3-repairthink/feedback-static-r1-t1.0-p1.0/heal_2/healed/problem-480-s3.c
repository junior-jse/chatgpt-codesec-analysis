#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

char findMaxOccurringChar(const char *str, size_t strSize)
{
    if (str == NULL || strSize == 0) {
        return '\0';
    }

    size_t len = strnlen(str, strSize);
    if (len == 0) {
        return '\0';
    }

    size_t count[UCHAR_MAX + 1] = {0};

    for (size_t i = 0; i < len; i++) {
        count[(unsigned char)str[i]]++;
    }

    size_t maxCount = 0;
    char maxChar = '\0';

    for (int j = 0; j <= UCHAR_MAX; j++) {
        if (count[j] > maxCount) {
            maxCount = count[j];
            maxChar = (char)j;
        }
    }

    return maxChar;
}

int main(void)
{
    char *str = NULL;
    size_t capacity = 0;

    printf("Enter a string: ");

    ssize_t nread = getline(&str, &capacity, stdin);
    if (nread < 0) {
        fprintf(stderr, "Error reading input\n");
        free(str);
        return 1;
    }

    size_t len = (size_t)nread;

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Empty string provided\n");
        free(str);
        return 1;
    }

    char maxChar = findMaxOccurringChar(str, len);

    if (maxChar != '\0') {
        printf("Maximum occurring character: '%c'\n", maxChar);
    } else {
        printf("No characters found\n");
    }

    free(str);
    return 0;
}