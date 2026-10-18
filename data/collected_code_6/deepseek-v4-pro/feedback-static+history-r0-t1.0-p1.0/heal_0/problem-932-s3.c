#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** remove_duplicates(char **words, int count, int *new_count) {
    if (words == NULL || count <= 0 || new_count == NULL) {
        if (new_count != NULL) *new_count = 0;
        return NULL;
    }
    
    char **result = (char**)malloc(count * sizeof(char*));
    if (result == NULL) {
        *new_count = 0;
        return NULL;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (words[i] == NULL) continue;
        
        int is_duplicate = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], result[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[unique_count] = (char*)malloc((strlen(words[i]) + 1) * sizeof(char));
            if (result[unique_count] == NULL) {
                for (int k = 0; k < unique_count; k++) {
                    free(result[k]);
                }
                free(result);
                *new_count = 0;
                return NULL;
            }
            strcpy(result[unique_count], words[i]);
            unique_count++;
        }
    }
    
    if (unique_count == 0) {
        free(result);
        *new_count = 0;
        return NULL;
    }
    
    char **final_result = (char**)realloc(result, unique_count * sizeof(char*));
    if (final_result == NULL && unique_count > 0) {
        for (int k = 0; k < unique_count; k++) {
            free(result[k]);
        }
        free(result);
        *new_count = 0;
        return NULL;
    }
    
    *new_count = unique_count;
    return final_result;
}

void free_string_array(char **array, int count) {
    if (array == NULL) return;
    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
    free(array);
}

int main() {
    char *words[] = {"apple", "banana", "apple", "orange", "banana", "grape", "apple"};
    int count = sizeof(words) / sizeof(words[0]);
    int new_count = 0;
    
    char **unique_words = remove_duplicates(words, count, &new_count);
    
    if (unique_words != NULL) {
        printf("Unique words: ");
        for (int i = 0; i < new_count; i++) {
            printf("%s", unique_words[i]);
            if (i < new_count - 1) printf(", ");
        }
        printf("\n");
        free_string_array(unique_words, new_count);
    } else {
        printf("No unique words found or memory allocation failed.\n");
    }
    
    return 0;
}