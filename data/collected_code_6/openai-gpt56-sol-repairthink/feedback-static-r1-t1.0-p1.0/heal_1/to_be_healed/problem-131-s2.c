#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_vowel(unsigned char c)
{
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

static void reverse_vowels(char *str)
{
    size_t left = 0;
    size_t right;

    if (str == NULL) {
        return;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    right = strlen(str);

    while (left < right) {
        while (left < right && !is_vowel((unsigned char)str[left])) {
            ++left;
        }

        while (left < right && !is_vowel((unsigned char)str[right - 1])) {
            --right;
        }

        if (left < right) {
            char temp = str[left];
            str[left] = str[right - 1];
            str[right - 1] = temp;
            ++left;
            --right;
        }
    }
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length = getline(&line, &capacity, stdin);

    if (length < 0) {
        free(line);
        return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    if (length > 0 && line[length - 1] == '\n') {
        line[--length] = '\0';
    }

    reverse_vowels(line);

    if (puts(line) == EOF) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}