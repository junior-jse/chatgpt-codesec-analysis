#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void remove_digits(char **list, size_t size) {
    if (list == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        
        size_t read = 0, write = 0;
        while (list[i][read] != '\0') {
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
    char *strings[] = {
        strdup("hello123world"),
        strdup("abc456def"),
        strdup("no_digits_here"),
        strdup("12345"),
        NULL
    };
    
    if (strings[0] == NULL || strings[1] == NULL || 
        strings[2] == NULL || strings[3] == NULL) {
        for (size_t i = 0; i < 4; i++) {
            free(strings[i]);
        }
        return EXIT_FAILURE;
    }
    
    size_t size = 4;
    
    printf("Before:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }
    
    remove_digits(strings, size);
    
    printf("\nAfter:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }
    
    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
    }
    
    return EXIT_SUCCESS;
}