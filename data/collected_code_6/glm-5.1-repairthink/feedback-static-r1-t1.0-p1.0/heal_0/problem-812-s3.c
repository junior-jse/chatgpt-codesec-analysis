#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *abbreviate_road(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char *result = malloc(len + 1);
    
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;

    while (i < len) {
        if (str[i] == 'r' && (i + 3) < len && str[i+1] == 'o' && str[i+2] == 'a' && str[i+3] == 'd') {
            result[j++] = 'r';
            result[j++] = 'd';
            result[j++] = '.';
            i += 4;
        } else {
            result[j++] = str[i++];
        }
    }

    result[j] = '\0';

    char *shrunk = realloc(result, j + 1);
    
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    const char *input = "I walked down the long road to find the roadblock on the road.";
    
    char *output = abbreviate_road(input);
    
    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    return 0;
}