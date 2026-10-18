#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int is_vowel(unsigned char character)
{
    switch (tolower(character)) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return 1;
        default:
            return 0;
    }
}

static void reverse_vowels(char *string)
{
    size_t left = 0;
    size_t right;

    if (string == NULL) {
        return;
    }

    right = strlen(string);

    if (right == 0) {
        return;
    }

    --right;

    while (left < right) {
        while (left < right && !is_vowel((unsigned char)string[left])) {
            ++left;
        }

        while (left < right && !is_vowel((unsigned char)string[right])) {
            --right;
        }

        if (left < right) {
            char temporary = string[left];
            string[left] = string[right];
            string[right] = temporary;
            ++left;
            --right;
        }
    }
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    length = getline(&line, &capacity, stdin);
    if (length < 0) {
        free(line);
        return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    if (length > 0 && line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }

    reverse_vowels(line);

    if (puts(line) == EOF) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}