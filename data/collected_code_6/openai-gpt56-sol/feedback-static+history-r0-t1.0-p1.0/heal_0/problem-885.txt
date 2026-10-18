#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static bool are_isomorphic(const char *first, const char *second)
{
    if (first == NULL || second == NULL) {
        return false;
    }

    size_t first_length = strlen(first);
    size_t second_length = strlen(second);

    if (first_length != second_length) {
        return false;
    }

    unsigned char first_to_second[256] = {0};
    unsigned char second_to_first[256] = {0};
    bool first_mapped[256] = {false};
    bool second_mapped[256] = {false};

    for (size_t i = 0; i < first_length; ++i) {
        unsigned char first_char = (unsigned char)first[i];
        unsigned char second_char = (unsigned char)second[i];

        if (first_mapped[first_char]) {
            if (first_to_second[first_char] != second_char) {
                return false;
            }
        } else {
            if (second_mapped[second_char]) {
                return false;
            }

            first_to_second[first_char] = second_char;
            second_to_first[second_char] = first_char;
            first_mapped[first_char] = true;
            second_mapped[second_char] = true;
        }
    }

    return true;
}

int main(void)
{
    char first[1024];
    char second[1024];

    if (fgets(first, sizeof(first), stdin) == NULL) {
        return 1;
    }

    if (fgets(second, sizeof(second), stdin) == NULL) {
        return 1;
    }

    size_t first_length = strlen(first);
    size_t second_length = strlen(second);

    if (first_length > 0 && first[first_length - 1] == '\n') {
        first[first_length - 1] = '\0';
    } else if (first_length == sizeof(first) - 1 && !feof(stdin)) {
        return 1;
    }

    if (second_length > 0 && second[second_length - 1] == '\n') {
        second[second_length - 1] = '\0';
    } else if (second_length == sizeof(second) - 1 && !feof(stdin)) {
        return 1;
    }

    if (ferror(stdin)) {
        return 1;
    }

    puts(are_isomorphic(first, second) ? "Isomorphic" : "Not isomorphic");

    if (ferror(stdout)) {
        return 1;
    }

    return 0;
}