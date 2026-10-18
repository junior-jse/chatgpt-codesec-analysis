#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_vowel(unsigned char c)
{
    switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            return 1;
        default:
            return 0;
    }
}

static void reverse_vowels(char *str)
{
    size_t left;
    size_t right;
    char temp;

    if (str == NULL) {
        return;
    }

    left = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    right = strlen(str);

    if (right == 0) {
        return;
    }

    --right;

    while (left < right) {
        while (left < right && !is_vowel((unsigned char)str[left])) {
            ++left;
        }

        while (left < right && !is_vowel((unsigned char)str[right])) {
            --right;
        }

        if (left < right) {
            temp = str[left];
            str[left] = str[right];
            str[right] = temp;
            ++left;
            --right;
        }
    }
}

int main(void)
{
    char *line;
    size_t capacity;
    size_t length;
    int ch;
    char *resized;

    capacity = 128;
    length = 0;
    line = malloc(capacity);

    if (line == NULL) {
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (length + 1 >= capacity) {
            if (capacity > SIZE_MAX / 2) {
                free(line);
                return EXIT_FAILURE;
            }

            capacity *= 2;
            resized = realloc(line, capacity);

            if (resized == NULL) {
                free(line);
                return EXIT_FAILURE;
            }

            line = resized;
        }

        line[length++] = (char)ch;
    }

    if (ferror(stdin)) {
        free(line);
        return EXIT_FAILURE;
    }

    line[length] = '\0';
    reverse_vowels(line);

    if (puts(line) == EOF) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}