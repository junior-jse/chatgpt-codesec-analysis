#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse_range(char *start, char *end) {
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

    size_t len = strnlen(str, 4096);
    if (len == 0 || len >= 4096) {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    if (len + 1 <= len) {
        free(result);
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result, str, len);
    result[len] = '\0';

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