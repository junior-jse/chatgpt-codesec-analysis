#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *abbreviate_road(const char *str) {
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (!result) {
        return NULL;
    }
    if (len == 0) {
        result[0] = '\0';
        return result;
    }
    size_t j = 0;
    for (size_t i = 0; i < len; ) {
        if (str[i] == 'r' && i + 3 < len && str[i+1] == 'o' && str[i+2] == 'a' && str[i+3] == 'd') {
            if ((i == 0 || str[i-1] == ' ') && (i + 4 == len || str[i+4] == ' ')) {
                result[j++] = 'r';
                result[j++] = 'd';
                result[j++] = '.';
                i += 4;
                continue;
            }
        }
        result[j++] = str[i++];
    }
    result[j] = '\0';
    char *shrunk = realloc(result, j + 1);
    return shrunk ? shrunk : result;
}

int main(void) {
    const char *input = "I walked down the long road to find the road ahead";
    char *output = abbreviate_road(input);
    if (output) {
        printf("%s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    return 0;
}