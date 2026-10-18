#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int find_sequences(const char *input, char ***matches, int *count) {
    regex_t regex;
    regmatch_t pmatch[1];
    const char *pattern = "[A-Z][a-z]+";
    int ret;
    int max_matches = 10;
    int found = 0;
    char **result;
    const char *p = input;

    if (input == NULL || matches == NULL || count == NULL) {
        return -1;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        return -1;
    }

    result = (char **)malloc(max_matches * sizeof(char *));
    if (result == NULL) {
        regfree(&regex);
        return -1;
    }

    p = input;
    while (regexec(&regex, p, 1, pmatch, 0) == 0) {
        if (found >= max_matches) {
            max_matches *= 2;
            char **temp = (char **)realloc(result, max_matches * sizeof(char *));
            if (temp == NULL) {
                for (int i = 0; i < found; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                return -1;
            }
            result = temp;
        }

        if (pmatch[0].rm_so < 0 || pmatch[0].rm_eo < 0) {
            for (int i = 0; i < found; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return -1;
        }

        int len = pmatch[0].rm_eo - pmatch[0].rm_so;
        if (len <= 0) {
            for (int i = 0; i < found; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return -1;
        }

        size_t remaining = strlen(p);
        if ((size_t)pmatch[0].rm_so >= remaining || (size_t)pmatch[0].rm_eo > remaining) {
            for (int i = 0; i < found; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return -1;
        }

        if ((size_t)len > remaining - pmatch[0].rm_so) {
            for (int i = 0; i < found; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return -1;
        }

        result[found] = (char *)malloc((size_t)(len + 1));
        if (result[found] == NULL) {
            for (int i = 0; i < found; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return -1;
        }

        memcpy(result[found], p + pmatch[0].rm_so, (size_t)len);
        result[found][len] = '\0';
        found++;

        p += pmatch[0].rm_eo;
    }

    *matches = result;
    *count = found;
    regfree(&regex);
    return 0;
}

int main() {
    const char *text = "Hello World This Is A Test String With Uppercase Words Like ABCdef and Xyz";
    char **matches = NULL;
    int count = 0;

    if (find_sequences(text, &matches, &count) == 0) {
        printf("Found %d sequences:\n", count);
        for (int i = 0; i < count; i++) {
            printf("%s\n", matches[i]);
            free(matches[i]);
        }
        free(matches);
    } else {
        printf("Error finding sequences\n");
    }

    return 0;
}