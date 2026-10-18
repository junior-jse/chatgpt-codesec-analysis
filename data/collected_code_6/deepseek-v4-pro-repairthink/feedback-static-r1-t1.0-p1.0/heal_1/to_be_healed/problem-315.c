#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* first_max_even_word(const char* str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    static char result[256];
    const char* word_start = NULL;
    size_t max_len = 0;
    size_t current_len = 0;
    const char* p = str;

    while (*p != '\0') {
        if (isalpha((unsigned char)*p)) {
            if (word_start == NULL) {
                word_start = p;
                current_len = 0;
            }
            current_len++;
        } else {
            if (word_start != NULL && current_len % 2 == 0 && current_len > max_len) {
                max_len = current_len;
                size_t copy_len = current_len < 255 ? current_len : 255;
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result, word_start, copy_len);
                result[copy_len] = '\0';
            }
            word_start = NULL;
            current_len = 0;
        }
        p++;
    }

    if (word_start != NULL && current_len % 2 == 0 && current_len > max_len) {
        size_t copy_len = current_len < 255 ? current_len : 255;
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result, word_start, copy_len);
        result[copy_len] = '\0';
        return result;
    }

    if (max_len > 0) {
        return result;
    }

    return NULL;
}

int main(void) {
    const char* text = "This is a sample sentence with words of varied lengths";
    char* word = first_max_even_word(text);

    if (word != NULL) {
        printf("First maximum length even word: %s\n", word);
    } else {
        printf("No even length word found.\n");
    }

    return 0;
}