#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

#define INITIAL_CAPACITY 128

char findMaxOccurringChar(const char *str, int *maxCount)
{
    int freq[256] = {0};
    size_t i;
    int max = 0;
    char result = '\0';

    if (str == NULL || maxCount == NULL) {
        if (maxCount != NULL) {
            *maxCount = 0;
        }
        return '\0';
    }

    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        if (freq[c] < INT_MAX) {
            freq[c]++;
        }
    }

    for (i = 0; i < 256; i++) {
        if (freq[i] > max) {
            max = freq[i];
            result = (char)i;
        }
    }

    *maxCount = max;
    return result;
}

int main(void)
{
    size_t capacity = INITIAL_CAPACITY;
    size_t length = 0;
    char *buffer;
    int maxCount = 0;
    char maxChar;
    int ch;

    buffer = malloc(capacity);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter a string: ");
    fflush(stdout);

    while ((ch = getchar()) != EOF && ch != '\n') {
        if (length + 1 >= capacity) {
            size_t newCapacity;
            char *resized;

            if (capacity > SIZE_MAX / 2) {
                fprintf(stderr, "Input too large\n");
                free(buffer);
                return 1;
            }

            newCapacity = capacity * 2;
            resized = realloc(buffer, newCapacity);
            if (resized == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(buffer);
                return 1;
            }

            buffer = resized;
            capacity = newCapacity;
        }

        buffer[length] = (char)ch;
        length++;
    }

    buffer[length] = '\0';

    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        free(buffer);
        return 1;
    }

    if (length == 0) {
        printf("Empty string provided\n");
        free(buffer);
        return 0;
    }

    maxChar = findMaxOccurringChar(buffer, &maxCount);

    if (maxCount > 0) {
        if (isprint((unsigned char)maxChar)) {
            printf("Maximum occurring character: '%c' (appears %d times)\n",
                   maxChar, maxCount);
        } else {
            printf("Maximum occurring character: '\\x%02X' (appears %d times)\n",
                   (unsigned char)maxChar, maxCount);
        }
    } else {
        printf("No characters found\n");
    }

    free(buffer);
    return 0;
}