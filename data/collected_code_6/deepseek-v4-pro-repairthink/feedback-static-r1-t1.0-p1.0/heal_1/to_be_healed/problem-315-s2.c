#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int first_max_even_word(const char *str, char *result, size_t result_size) {
    if (str == NULL || result == NULL || result_size == 0) {
        return -1;
    }

    size_t max_len = 0;
    size_t max_start = 0;
    int found = 0;
    size_t i = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);

    while (i <= len) {
        if (i == len || isspace((unsigned char)str[i])) {
            size_t word_len = i - max_start;
            if (word_len > 0 && word_len % 2 == 0) {
                if (!found || word_len > max_len) {
                    max_len = word_len;
                    /* Possible weaknesses found:
                     *  Value stored to 'max_start' is never read [deadcode.DeadStores]
                     *  max_start is assigned
                     */
                    max_start = i - word_len;
                    found = 1;
                }
            }
            /* Possible weaknesses found:
             *  max_start is overwritten
             *  Variable 'max_start' is reassigned a value before the old one has been used. [redundantAssignment]
             */
            max_start = i + 1;
        }
        i++;
    }

    if (!found) {
        result[0] = '\0';
        return 0;
    }

    if (max_len >= result_size) {
        max_len = result_size - 1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result, str + max_start, max_len);
    result[max_len] = '\0';

    return 1;
}

int main(void) {
    const char *text = "The quick brown fox jumps over the lazy dog";
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[100];

    int ret = first_max_even_word(text, word, sizeof(word));

    if (ret == 1) {
        printf("First max even word: %s\n", word);
    } else if (ret == 0) {
        printf("No even word found.\n");
    } else {
        printf("Invalid input.\n");
    }

    return 0;
}