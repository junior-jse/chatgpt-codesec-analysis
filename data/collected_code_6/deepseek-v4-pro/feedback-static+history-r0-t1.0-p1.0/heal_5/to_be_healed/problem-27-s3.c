#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char *remove_digits(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
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
    char **list = calloc(count, sizeof(char *));
    if (list == NULL) {
        return EXIT_FAILURE;
    }
    
    const char *strings[] = {"h3ll0", "w0rld123", "c0d1ng", "t3st456"};
    
    for (size_t i = 0; i < count; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len = strlen(strings[i]);
        
        list[i] = malloc(len + 1);
        if (list[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return EXIT_FAILURE;
        }
        
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(list[i], strings[i], len + 1);
        list[i][len] = '\0';
    }
    
    if (remove_all_digits(list, count) != 0) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            printf("%s\n", list[i]);
        }
    }
    
    for (size_t i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);
    
    return EXIT_SUCCESS;
}