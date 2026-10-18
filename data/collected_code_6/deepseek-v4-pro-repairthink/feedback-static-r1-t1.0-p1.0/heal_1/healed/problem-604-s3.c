#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 4096

static size_t bounded_strlen(const char *str, size_t maxlen) {
    size_t len = 0;
    while (len < maxlen && str[len] != '\0') {
        len++;
    }
    return len;
}

static void reverse_range(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

char *reverse_words(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = bounded_strlen(input, MAX_STRING_LEN);
    if (len == MAX_STRING_LEN) {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, input, len + 1);
    reverse_range(result, result + len - 1);

    char *word_start = result;
    char *current = result;

    while (*current != '\0') {
        if (*current == ' ') {
            reverse_range(word_start, current - 1);
            word_start = current + 1;
        }
        current++;
    }

    reverse_range(word_start, current - 1);

    return result;
}

int main(void) {
    const char *test_cases[] = {
        "Hello World",
        "The quick brown fox",
        "Single",
        "  Multiple   spaces  ",
        "",
        "C programming is fun"
    };
    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char *reversed = reverse_words(test_cases[i]);
        if (reversed != NULL) {
            printf("Input: \"%s\"\nReversed words: \"%s\"\n\n", test_cases[i], reversed);
            free(reversed);
        } else {
            printf("Memory allocation failed or invalid input for test case %zu\n", i);
        }
    }

    return 0;
}