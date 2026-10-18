#include <stdio.h>
#include <stdlib.h>

#define ASCII_SIZE 128

char find_max_char(const char *str) {
    int count[ASCII_SIZE] = {0};
    int max_count = 0;
    char max_char = '\0';

    if (str == NULL) {
        return '\0';
    }

    for (const char *p = str; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;
        if (c >= ASCII_SIZE) {
            continue;
        }
        count[c]++;
        if (count[c] > max_count) {
            max_count = count[c];
            max_char = c;
        }
    }

    return max_char;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char result = find_max_char(argv[1]);
    if (result != '\0') {
        printf("%c\n", result);
    } else {
        printf("No valid characters found or empty string provided.\n");
    }

    return EXIT_SUCCESS;
}