#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

char *reverse_vowels(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    size_t len = strlen(s);
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, s, len + 1);

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
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