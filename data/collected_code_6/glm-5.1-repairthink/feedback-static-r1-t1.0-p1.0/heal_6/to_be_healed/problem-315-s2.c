#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* find_first_max_even_word(const char* str, size_t* out_len) {
    if (str == NULL || out_len == NULL) {
        return NULL;
    }

    *out_len = 0;
    size_t i = 0;
    size_t best_start = 0;
    size_t best_len = 0;

    while (str[i] != '\0') {
        size_t start = i;
        while (str[i] != '\0' && str[i] != ' ') {
            i++;
        }
        size_t word_len = i - start;

        if (word_len > 0 && word_len % 2 == 0) {
            if (word_len > best_len) {
                best_len = word_len;
                best_start = start;
                *out_len = word_len;
            }
        }

        if (str[i] != '\0') {
            i++;
        }
    }

    if (best_len == 0) {
        return NULL;
    }

    if (best_len == SIZE_MAX) {
        return NULL;
    }

    /* Possible weaknesses found:
     *  alloc_size is assigned 'best_len+1' here.
     */
    size_t alloc_size = best_len + 1;

    char* result = malloc(alloc_size);
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (result == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     *  copy_len is assigned 'best_len' here.
     */
    size_t copy_len = best_len;
    /* Possible weaknesses found:
     *  Condition 'copy_len>=alloc_size' is always false
     *  Condition 'copy_len>=alloc_size' is always false [knownConditionTrueFalse]
     */
    if (copy_len >= alloc_size) {
        copy_len = alloc_size - 1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result, str + best_start, copy_len);
    result[copy_len] = '\0';

    return result;
}

int main(void) {
    const char* text = "This is an example string with some long even words like eight";
    size_t max_len = 0;
    
    char* word = find_first_max_even_word(text, &max_len);
    
    if (word != NULL) {
        printf("First max even word: %s\n", word);
        printf("Length: %zu\n", max_len);
        free(word);
    } else {
        printf("No even length word found.\n");
    }
    
    return 0;
}