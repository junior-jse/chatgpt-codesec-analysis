#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* remove_lowercase_substrings(const char* input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t input_len = strlen(input);
    char* result = (char*)malloc(input_len + 1);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t result_idx = 0;
    const char* cursor = input;

    while (*cursor != '\0') {
        regmatch_t match;
        ret = regexec(&regex, cursor, 1, &match, 0);

        if (ret == 0) {
            for (regoff_t i = 0; i < match.rm_so; ++i) {
                result[result_idx++] = cursor[i];
            }
            cursor += match.rm_eo;
        } else {
            while (*cursor != '\0') {
                result[result_idx++] = *cursor++;
            }
        }
    }

    result[result_idx] = '\0';
    regfree(&regex);

    char* shrunk_result = (char*)realloc(result, result_idx + 1);
    if (shrunk_result != NULL) {
        return shrunk_result;
    }
    
    return result;
}

int main(void) {
    char* str1 = remove_lowercase_substrings("HelloWorld");
    if (str1 != NULL) {
        printf("%s\n", str1);
        free(str1);
    }

    char* str2 = remove_lowercase_substrings("ABCdefGHIjklMNO");
    if (str2 != NULL) {
        printf("%s\n", str2);
        free(str2);
    }

    char* str3 = remove_lowercase_substrings("1234567890");
    if (str3 != NULL) {
        printf("%s\n", str3);
        free(str3);
    }

    char* str4 = remove_lowercase_substrings(NULL);
    if (str4 == NULL) {
        printf("NULL input handled\n");
    }

    return 0;
}