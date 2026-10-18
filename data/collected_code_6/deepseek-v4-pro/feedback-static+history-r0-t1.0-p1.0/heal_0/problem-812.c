#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *abbreviate_road(const char *input) {
    size_t len = strlen(input);
    char *result = malloc(len + 1);
    if (!result) return NULL;
    
    size_t i = 0, j = 0;
    while (i < len) {
        if (i + 3 < len && 
            tolower(input[i]) == 'r' && 
            tolower(input[i+1]) == 'o' && 
            tolower(input[i+2]) == 'a' && 
            tolower(input[i+3]) == 'd' &&
            (i == 0 || !isalpha(input[i-1])) &&
            (i + 4 >= len || !isalpha(input[i+4]))) {
            result[j++] = 'r';
            result[j++] = 'd';
            result[j++] = '.';
            i += 4;
        } else {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    
    char *shrunk = realloc(result, j + 1);
    return shrunk ? shrunk : result;
}

int main(void) {
    const char *test_cases[] = {
        "Main road",
        "Road to nowhere",
        "Railroad crossing",
        "road",
        "Road",
        "No roads here",
        "Broadway road rd.",
        "roadrunner"
    };
    
    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        char *abbrev = abbreviate_road(test_cases[i]);
        if (abbrev) {
            printf("\"%s\" -> \"%s\"\n", test_cases[i], abbrev);
            free(abbrev);
        } else {
            printf("Memory allocation failed for \"%s\"\n", test_cases[i]);
        }
    }
    
    return 0;
}