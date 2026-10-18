#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_characters(char *start, char *end) {
    if (start == NULL || end == NULL || start > end) {
        return;
    }
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverse_words(char *str) {
    if (str == NULL) {
        return;
    }
    size_t len = strnlen(str, 256);
    if (len == 0) {
        return;
    }
    reverse_characters(str, str + len - 1);
    char *word_start = str;
    char *word_end = str;
    while (*word_end != '\0') {
        if (*word_end == ' ') {
            if (word_start < word_end) {
                reverse_characters(word_start, word_end - 1);
            }
            word_end++;
            word_start = word_end;
        } else {
            word_end++;
        }
    }
    if (word_start < word_end) {
        reverse_characters(word_start, word_end - 1);
    }
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256];
    buffer[0] = '\0';
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t len = strnlen(buffer, sizeof(buffer));
    if (len > 0 && len < sizeof(buffer) && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    reverse_words(buffer);
    printf("%s\n", buffer);
    return EXIT_SUCCESS;
}