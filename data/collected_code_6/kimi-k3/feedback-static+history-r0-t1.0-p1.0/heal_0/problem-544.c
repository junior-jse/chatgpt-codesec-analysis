#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    double value;
} Tuple;

char* flatten_tuple_list(const Tuple* tuples, size_t count) {
    if (tuples == NULL || count == 0) {
        char* empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }
    
    size_t total_size = 1;
    for (size_t i = 0; i < count; i++) {
        total_size += snprintf(NULL, 0, "(%d,%s,%.2f)", 
                               tuples[i].id, tuples[i].name, tuples[i].value) + 1;
    }
    
    char* result = malloc(total_size);
    if (result == NULL) {
        return NULL;
    }
    
    result[0] = '\0';
    size_t offset = 0;
    
    for (size_t i = 0; i < count; i++) {
        int written = snprintf(result + offset, total_size - offset, 
                               "(%d,%s,%.2f)", 
                               tuples[i].id, tuples[i].name, tuples[i].value);
        if (written < 0 || (size_t)written >= total_size - offset) {
            free(result);
            return NULL;
        }
        offset += (size_t)written;
        
        if (i < count - 1) {
            if (offset + 1 >= total_size) {
                free(result);
                return NULL;
            }
            result[offset] = ',';
            offset++;
            result[offset] = '\0';
        }
    }
    
    return result;
}

int main(void) {
    Tuple tuples[] = {
        {1, "apple", 2.5},
        {2, "banana", 1.75},
        {3, "cherry", 3.0}
    };
    
    size_t count = sizeof(tuples) / sizeof(tuples[0]);
    char* flattened = flatten_tuple_list(tuples, count);
    
    if (flattened == NULL) {
        fprintf(stderr, "Error: Failed to flatten tuple list\n");
        return EXIT_FAILURE;
    }
    
    printf("%s\n", flattened);
    free(flattened);
    
    return EXIT_SUCCESS;
}