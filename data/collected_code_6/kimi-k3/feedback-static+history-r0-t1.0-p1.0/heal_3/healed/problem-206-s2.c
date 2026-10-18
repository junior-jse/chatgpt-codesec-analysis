#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concatenate_adjacent(const char* tuple[], int size) {
    if (size <= 0 || tuple == NULL) {
        return NULL;
    }
    
    size_t total_len = 0;
    int i;
    
    for (i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        total_len += strnlen(tuple[i], 1024);
    }
    
    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t offset = 0;
    
    for (i = 0; i < size - 1; i++) {
        size_t len_i = strnlen(tuple[i], total_len - offset);
        if (offset + len_i > total_len) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple[i], len_i);
        offset += len_i;
        
        size_t len_next = strnlen(tuple[i + 1], total_len - offset);
        if (offset + len_next > total_len) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple[i + 1], len_next);
        offset += len_next;
    }
    
    result[offset] = '\0';
    
    return result;
}

int main(void) {
    const char* tuple[] = {"Hello", "World", "C", "Programming"};
    int size = sizeof(tuple) / sizeof(tuple[0]);
    
    char* concatenated = concatenate_adjacent(tuple, size);
    
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
        concatenated = NULL;
    } else {
        fprintf(stderr, "Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}