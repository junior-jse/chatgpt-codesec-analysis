#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_non_alnum(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strlen(input);
    char *result = malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[^a-zA-Z0-9]", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return NULL;
    }

    size_t result_idx = 0;
    const char *ptr = input;

    while (*ptr != '\0') {
        regmatch_t match;
        int match_ret = regexec(&regex, ptr, 1, &match, 0);
        
        if (match_ret == 0) {
            for (regoff_t i = 0; i < match.rm_so; i++) {
                result[result_idx++] = ptr[i];
            }
            ptr += match.rm_eo;
        } else {
            while (*ptr != '\0') {
                result[result_idx++] = *ptr++;
            }
        }
    }

    result[result_idx] = '\0';
    regfree(&regex);

    char *shrunk = realloc(result, result_idx + 1);
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    char *cleaned = remove_non_alnum("H3ll0, W@rld! 123.");
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    cleaned = remove_non_alnum("___---((()))___");
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    cleaned = remove_non_alnum("NoSpecialChars123");
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    return 0;
}