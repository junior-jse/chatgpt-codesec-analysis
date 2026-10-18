#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse(char *start, char *end) {
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

    reverse(str, str + len - 1);

    char *word_start = str;
    char *word_end = str;

    while (*word_end) {
        if (*word_end == ' ') {
            if (word_start != word_end) {
                reverse(word_start, word_end - 1);
            }
            word_end++;
            word_start = word_end;
        } else {
            word_end++;
        }
    }

    if (word_start != word_end) {
        reverse(word_start, word_end - 1);
    }
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }

    size_t length = strnlen(buffer, sizeof(buffer));
    if (length > 0 && length < sizeof(buffer) && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    reverse_words(buffer);

    printf("%s\n", buffer);

    return 0;
}