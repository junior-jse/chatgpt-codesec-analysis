#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char find_first_repeated(const char *str) {
    unsigned char seen[UCHAR_MAX + 1] = {0};

    if (str == NULL) {
        return '\0';
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        if (seen[c] == 1) {
            return str[i];
        }
        seen[c] = 1;
    }

    return '\0';
}

int main(int argc, const char * const argv[]) {
    char result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = find_first_repeated(argv[1]);

    if (result != '\0') {
        printf("%c\n", result);
    } else {
        printf("No repeated character found\n");
    }

    return EXIT_SUCCESS;
}