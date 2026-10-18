#include <stdio.h>
#include <string.h>
#include <limits.h>

char findMaxOccurringChar(const char *str, size_t max_len) {
    if (str == NULL || max_len == 0 || str[0] == '\0') {
        return '\0';
    }

    int count[256] = {0};
    size_t i;
    size_t len = strnlen(str, max_len);

    for (i = 0; i < len; i++) {
        count[(unsigned char)str[i]]++;
    }

    int maxCount = 0;
    char maxChar = '\0';
    int j;

    for (j = 0; j < 256; j++) {
        if (count[j] > maxCount) {
            maxCount = count[j];
            maxChar = (char)j;
        }
    }

    return maxChar;
}

int main(void) {
    char str[1024];

    printf("Enter a string: ");
    if (fgets(str, sizeof(str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strnlen(str, sizeof(str));
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }

    if (str[0] == '\0') {
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