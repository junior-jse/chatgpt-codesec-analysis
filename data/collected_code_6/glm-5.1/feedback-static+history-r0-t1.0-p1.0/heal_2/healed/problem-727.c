#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_non_alnum(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }

    size_t result_size = len + 1;
    char *result = malloc(result_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if ((str[i] >= 'a' && str[i] <= 'z') || 
            (str[i] >= 'A' && str[i] <= 'Z') || 
            (str[i] >= '0' && str[i] <= '9')) {
            
            if (result_idx + 1 < result_size) {
                result[result_idx++] = str[i];
            }
        }
    }

    if (result_idx < result_size) {
        result[result_idx] = '\0';
    } else {
        result[result_size - 1] = '\0';
    }

    char *final_result = realloc(result, result_idx + 1);
    if (final_result == NULL) {
        final_result = result;
    }

    return final_result;
}

int main(void) {
    const char *input = "H3ll0 W0rld! @2024 #C_Programming*()";
    char *output = remove_non_alnum(input);
    
    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    }

    return 0;
}