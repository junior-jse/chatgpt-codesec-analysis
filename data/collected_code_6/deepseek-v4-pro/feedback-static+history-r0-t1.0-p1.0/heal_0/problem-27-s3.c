#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_digits(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
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
    return result;
}

int remove_all_digits(char **list, size_t size) {
    if (list == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        
        char *filtered = remove_digits(list[i]);
        if (filtered == NULL) {
            return -1;
        }
        
        free(list[i]);
        list[i] = filtered;
    }
    
    return 0;
}

int main(void) {
    size_t count = 4;
    char **list = malloc(count * sizeof(char *));
    if (list == NULL) {
        return EXIT_FAILURE;
    }
    
    list[0] = malloc(32);
    list[1] = malloc(32);
    list[2] = malloc(32);
    list[3] = malloc(32);
    
    if (list[0] == NULL || list[1] == NULL || list[2] == NULL || list[3] == NULL) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }
    
    strcpy(list[0], "h3ll0");
    strcpy(list[1], "w0rld123");
    strcpy(list[2], "c0d1ng");
    strcpy(list[3], "t3st456");
    
    if (remove_all_digits(list, count) != 0) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }
    
    for (size_t i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);
    
    return EXIT_SUCCESS;
}