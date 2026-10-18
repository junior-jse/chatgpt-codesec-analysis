#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match;
    int ret;
    const char *cursor = str;
    int found = 0;

    if (str == NULL) {
        fprintf(stderr, "Error: Input string is NULL\n");
        return -1;
    }

    ret = regcomp(&regex, "[A-Z][a-z]+", REG_EXTENDED);
    if (ret != 0) {
        size_t error_size = regerror(ret, &regex, NULL, 0);
        char *error_buffer = malloc(error_size);
        if (error_buffer == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            regfree(&regex);
            return -1;
        }
        regerror(ret, &regex, error_buffer, error_size);
        fprintf(stderr, "Error: Could not compile regex: %s\n", error_buffer);
        free(error_buffer);
        regfree(&regex);
        return -1;
    }

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            size_t error_size = regerror(ret, &regex, NULL, 0);
            char *error_buffer = malloc(error_size);
            if (error_buffer == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                regfree(&regex);
                return -1;
            }
            regerror(ret, &regex, error_buffer, error_size);
            fprintf(stderr, "Error: Regex execution failed: %s\n", error_buffer);
            free(error_buffer);
            regfree(&regex);
            return -1;
        }

        if (match.rm_so == -1) {
            break;
        }

        printf("Found sequence: %.*s\n", 
               (int)(match.rm_eo - match.rm_so), 
               cursor + match.rm_so);
        
        cursor += match.rm_eo;
        found = 1;
    }

    if (!found) {
        printf("No sequences found\n");
    }

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_string = "Hello World This Is A Test String with MixedCase Words";
    
    printf("Searching in: \"%s\"\n", test_string);
    
    if (find_sequences(test_string) != 0) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}