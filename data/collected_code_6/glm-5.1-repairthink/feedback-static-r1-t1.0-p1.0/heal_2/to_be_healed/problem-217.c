#include <stdio.h>
#include <stdlib.h>

char find_first_repeated(const char *str) {
    if (str == NULL) {
        return '\0';
    }

    unsigned char *char_set = calloc(256, sizeof(unsigned char));
    if (char_set == NULL) {
        return '\0';
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        if (char_set[c] == 1) {
            free(char_set);
            return c;
        }
        char_set[c] = 1;
    }

    free(char_set);
    return '\0';
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char result = find_first_repeated(argv[1]);
    if (result != '\0') {
        printf("%c\n", result);
    } else {
        printf("No repeated character found\n");
    }

    return EXIT_SUCCESS;
}