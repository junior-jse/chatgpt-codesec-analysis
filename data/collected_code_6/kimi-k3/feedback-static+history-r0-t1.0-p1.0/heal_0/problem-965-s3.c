#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    char *output = malloc(len * 2 + 1);
    if (output == NULL) {
        return NULL;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)input[i])) {
            if (i > 0) {
                output[j++] = '_';
            }
            output[j++] = (char)tolower((unsigned char)input[i]);
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    
    return output;
}

int main(void) {
    const char *test_strings[] = {
        "camelCaseString",
        "thisIsATest",
        "simpleTest",
        "already_snake",
        "lowercase",
        "A",
        "ABCDef"
    };
    
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        char *result = camel_to_snake(test_strings[i]);
        if (result != NULL) {
            printf("Input:  %s\n", test_strings[i]);
            printf("Output: %s\n\n", result);
            free(result);
        } else {
            fprintf(stderr, "Error converting: %s\n", test_strings[i]);
        }
    }
    
    return 0;
}