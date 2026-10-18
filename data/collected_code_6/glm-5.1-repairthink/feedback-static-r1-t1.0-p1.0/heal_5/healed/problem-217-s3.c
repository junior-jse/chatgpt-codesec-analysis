#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

char find_first_repeated(const char *str, size_t len) {
    unsigned char seen[UCHAR_MAX + 1] = {0};

    if (str == NULL) {
        return '\0';
    }

    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        unsigned int c = (unsigned char)str[i];
        if (c < sizeof(seen)) {
            if (seen[c] == 1) {
                return str[i];
            }
            seen[c] = 1;
        }
    }

    return '\0';
}

int main(int argc, const char * const argv[]) {
    char result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t len = strlen(argv[1]);
    result = find_first_repeated(argv[1], len);

    if (result != '\0') {
        printf("%c\n", result);
    } else {
        printf("No repeated character found\n");
    }

    return EXIT_SUCCESS;
}