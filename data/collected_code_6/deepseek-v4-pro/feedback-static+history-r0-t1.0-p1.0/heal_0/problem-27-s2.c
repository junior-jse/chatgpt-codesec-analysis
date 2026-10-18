#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_digits(const char *str) {
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    
    char *shrunk = realloc(result, j + 1);
    if (shrunk != NULL) {
        return shrunk;
    }
    return result;
}

void remove_digits_from_list(char **list, size_t count) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            char *cleaned = remove_digits(list[i]);
            if (cleaned != NULL) {
                free(list[i]);
                list[i] = cleaned;
            }
        }
    }
}

int main(void) {
    char **strings = malloc(4 * sizeof(char *));
    if (strings == NULL) {
        return EXIT_FAILURE;
    }
    
    strings[0] = malloc(20);
    strings[1] = malloc(20);
    strings[2] = malloc(20);
    strings[3] = malloc(20);
    
    if (strings[0] == NULL || strings[1] == NULL || 
        strings[2] == NULL || strings[3] == NULL) {
        for (int i = 0; i < 4; i++) {
            free(strings[i]);
        }
        free(strings);
        return EXIT_FAILURE;
    }
    
    strcpy(strings[0], "abc123def");
    strcpy(strings[1], "456");
    strcpy(strings[2], "no digits here");
    strcpy(strings[3], "a1b2c3d4e5");
    
    remove_digits_from_list(strings, 4);
    
    for (size_t i = 0; i < 4; i++) {
        printf("%s\n", strings[i]);
        free(strings[i]);
    }
    free(strings);
    
    return EXIT_SUCCESS;
}