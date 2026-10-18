#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replace_with_colon(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    if (len == 0) {
        char* empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t bufsize = len + 1;
    char* result = malloc(bufsize);
    if (result == NULL) {
        return NULL;
    }

    const char* p = str;
    size_t offset = 0;

    while (*p) {
        if (*p == ' ' || *p == ',' || *p == '.') {
            if (offset + 1 >= bufsize) {
                bufsize = (offset + 2) * 2;
                char* new_result = realloc(result, bufsize);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[offset++] = ':';
            p++;
        } else {
            if (offset + 1 >= bufsize) {
                bufsize = (offset + 2) * 2;
                char* new_result = realloc(result, bufsize);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[offset++] = *p;
            p++;
        }
    }

    result[offset] = '\0';

    return result;
}

int main(int argc, char* argv[]) {
    const char* input = "Hello world, this.is a test";
    
    char* output = replace_with_colon(input);
    if (output == NULL) {
        fprintf(stderr, "Regex replacement failed\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", output);

    free(output);
    return EXIT_SUCCESS;
}