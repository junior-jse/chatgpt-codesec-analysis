#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** convert_tuple_to_list(char** tuple, int size, const char* str) {
    if (tuple == NULL || str == NULL || size <= 0) {
        return NULL;
    }
    
    int new_size = size * 2;
    char** result = malloc(new_size * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        result[i * 2] = malloc(strlen(tuple[i]) + 1);
        if (result[i * 2] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        strcpy(result[i * 2], tuple[i]);
        
        result[i * 2 + 1] = malloc(strlen(str) + 1);
        if (result[i * 2 + 1] == NULL) {
            for (int j = 0; j <= i * 2; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        strcpy(result[i * 2 + 1], str);
    }
    
    return result;
}

void free_list(char** list, int size) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    char* tuple[] = {"apple", "banana", "cherry"};
    int tuple_size = 3;
    const char* separator = "fruit";
    
    char** result = convert_tuple_to_list(tuple, tuple_size, separator);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    int result_size = tuple_size * 2;
    for (int i = 0; i < result_size; i++) {
        printf("%s\n", result[i]);
    }
    
    free_list(result, result_size);
    
    return EXIT_SUCCESS;
}