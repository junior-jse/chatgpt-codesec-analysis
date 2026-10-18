#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define BUFFER_SIZE 4096
#define MAX_STRINGS 4

void remove_digits(char **list, size_t size) {
    if (list == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        
        size_t read = 0, write = 0;
        size_t len = strlen(list[i]);
        
        while (read < len && list[i][read] != '\0') {
            if (!isdigit((unsigned char)list[i][read])) {
                list[i][write] = list[i][read];
                write++;
            }
            read++;
        }
        list[i][write] = '\0';
    }
}

int main(void) {
    char *strings[MAX_STRINGS] = {NULL};
    const char *initial_values[] = {
        "hello123world",
        "abc456def",
        "no_digits_here",
        "12345"
    };
    
    for (size_t i = 0; i < MAX_STRINGS; i++) {
        strings[i] = strdup(initial_values[i]);
        if (strings[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
    }
    
    size_t size = MAX_STRINGS;
    
    printf("Before:\n");
    for (size_t i = 0; i < size; i++) {
        if (strings[i] != NULL) {
            printf("%s\n", strings[i]);
        }
    }
    
    remove_digits(strings, size);
    
    printf("\nAfter:\n");
    for (size_t i = 0; i < size; i++) {
        if (strings[i] != NULL) {
            printf("%s\n", strings[i]);
        }
    }
    
    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
    }
    
    return EXIT_SUCCESS;
}