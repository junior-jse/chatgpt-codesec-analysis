#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_words_containing(char **list, int size, const char *substring) {
    int i, j;
    int removed = 0;
    
    if (list == NULL || substring == NULL || size < 0) {
        return -1;
    }
    
    for (i = 0, j = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        if (strstr(list[i], substring) != NULL) {
            free(list[i]);
            list[i] = NULL;
            removed++;
        } else {
            list[j] = list[i];
            if (i != j) {
                list[i] = NULL;
            }
            j++;
        }
    }
    
    return j;
}

int main(void) {
    char **words;
    int size = 6;
    int new_size;
    int i;
    
    words = malloc(size * sizeof(char *));
    if (words == NULL) {
        return 1;
    }
    
    words[0] = malloc(6);
    words[1] = malloc(7);
    words[2] = malloc(7);
    words[3] = malloc(5);
    words[4] = malloc(11);
    words[5] = malloc(4);
    
    if (words[0] == NULL || words[1] == NULL || words[2] == NULL ||
        words[3] == NULL || words[4] == NULL || words[5] == NULL) {
        for (i = 0; i < size; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    snprintf(words[0], 6, "%s", "apple");
    snprintf(words[1], 7, "%s", "banana");
    snprintf(words[2], 7, "%s", "cherry");
    snprintf(words[3], 5, "%s", "date");
    snprintf(words[4], 11, "%s", "elderberry");
    snprintf(words[5], 4, "%s", "fig");
    
    printf("Original list:\n");
    for (i = 0; i < size; i++) {
        printf("%s\n", words[i]);
    }
    
    new_size = remove_words_containing(words, size, "an");
    
    if (new_size < 0) {
        for (i = 0; i < size; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    printf("\nList after removing words containing 'an':\n");
    for (i = 0; i < new_size; i++) {
        printf("%s\n", words[i]);
    }
    
    for (i = 0; i < size; i++) {
        free(words[i]);
    }
    free(words);
    
    return 0;
}