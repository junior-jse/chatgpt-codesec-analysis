#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static bool are_isomorphic(const char *first, const char *second)
{
    int first_to_second[256];
    int second_to_first[256];
    size_t i;

    if (first == NULL || second == NULL) {
        return false;
    }

    for (i = 0; i < 256; ++i) {
        first_to_second[i] = -1;
        second_to_first[i] = -1;
    }

    for (i = 0; first[i] != '\0' && second[i] != '\0'; ++i) {
        unsigned char first_char = (unsigned char)first[i];
        unsigned char second_char = (unsigned char)second[i];

        if (first_to_second[first_char] == -1 &&
            second_to_first[second_char] == -1) {
            first_to_second[first_char] = (int)second_char;
            second_to_first[second_char] = (int)first_char;
        } else if (first_to_second[first_char] != (int)second_char ||
                   second_to_first[second_char] != (int)first_char) {
            return false;
        }
    }

    return first[i] == '\0' && second[i] == '\0';
}

int main(void)
{
    char first[1024];
    char second[1024];

    if (fgets(first, sizeof first, stdin) == NULL) {
        return 1;
    }

    if (fgets(second, sizeof second, stdin) == NULL) {
        return 1;
    }

    if (strchr(first, '\n') == NULL && !feof(stdin)) {
        return 1;
    }

    if (strchr(second, '\n') == NULL && !feof(stdin)) {
        return 1;
    }

    first[strcspn(first, "\r\n")] = '\0';
    second[strcspn(second, "\r\n")] = '\0';

    puts(are_isomorphic(first, second) ? "Isomorphic" : "Not isomorphic");

    return 0;
}