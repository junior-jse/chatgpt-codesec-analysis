#include <limits.h>
#include <stdio.h>

int find_first_repeated_character(const char *str, unsigned char *result)
{
    unsigned char seen[UCHAR_MAX + 1] = {0};

    if (str == NULL || result == NULL) {
        return -1;
    }

    while (*str != '\0') {
        unsigned char ch = (unsigned char)*str;

        if (seen[ch] != 0U) {
            *result = ch;
            return 1;
        }

        seen[ch] = 1U;
        ++str;
    }

    return 0;
}

int main(void)
{
    char input[1024];
    unsigned char repeated;
    int status;

    if (fgets(input, sizeof input, stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return 1;
        }
        return 0;
    }

    status = find_first_repeated_character(input, &repeated);
    if (status < 0) {
        fprintf(stderr, "Invalid argument.\n");
        return 1;
    }

    if (status == 1) {
        printf("%c\n", repeated);
    } else {
        printf("No repeated character found.\n");
    }

    return 0;
}