#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void reverse_range(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

char *reverse_words(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (len < 4096 && str[len] != '\0') {
        len++;
    }

    if (len == 0 || len >= 4096) {
        return NULL;
    }

    if (len + 1 <= len) {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i <= len; i++) {
        result[i] = str[i];
    }

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
    char input[] = "Hello World from C";
    char *reversed = reverse_words(input);

    if (reversed != NULL) {
        printf("Original: %s\n", input);
        printf("Reversed: %s\n", reversed);
        free(reversed);
    }

    return 0;
}