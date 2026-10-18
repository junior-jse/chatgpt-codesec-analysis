#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

char *reverse_vowels(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (s[len] != '\0') {
        if (len == SIZE_MAX) {
            return NULL;
        }
        len++;
    }

    if (len + 1 <= len) {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i <= len; ++i) {
        result[i] = s[i];
    }

    size_t left = 0;
    size_t right = len > 0 ? len - 1 : 0;

    while (left < right) {
        if (!is_vowel(result[left])) {
            left++;
            continue;
        }
        if (!is_vowel(result[right])) {
            right--;
            continue;
        }

        char temp = result[left];
        result[left] = result[right];
        result[right] = temp;

        left++;
        right--;
    }

    return result;
}

int main(int argc, const char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0] ? argv[0] : "program");
        return EXIT_FAILURE;
    }

    char *reversed = reverse_vowels(argv[1]);
    if (reversed == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", reversed);

    free(reversed);
    return EXIT_SUCCESS;
}