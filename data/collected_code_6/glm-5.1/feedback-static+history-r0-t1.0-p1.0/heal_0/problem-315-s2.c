#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* find_first_max_even_word(const char* str, size_t* out_len) {
    if (str == NULL || out_len == NULL) {
        return NULL;
    }

    *out_len = 0;
    char* result = NULL;
    size_t len = strlen(str);
    size_t i = 0;

    while (i < len) {
        size_t start = i;
        while (i < len && str[i] != ' ') {
            i++;
        }
        size_t word_len = i - start;

        if (word_len > 0 && word_len % 2 == 0) {
            if (word_len > *out_len) {
                *out_len = word_len;
                free(result);
                result = malloc(word_len + 1);
                if (result == NULL) {
                    return NULL;
                }
                memcpy(result, &str[start], word_len);
                result[word_len] = '\0';
            }
        }

        i++;
    }

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