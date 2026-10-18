#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_at_uppercase(const char *input) {
    regex_t regex;
    regmatch_t match;
    int count = 1;
    const char *p = input;
    char **result;
    int i = 0;
    int start = 0;
    int len = strlen(input);

    if (len == 0) {
        result = malloc(2 * sizeof(char *));
        if (!result) {
            return NULL;
        }
        result[0] = malloc(1);
        if (!result[0]) {
            free(result);
            return NULL;
        }
        result[0][0] = '\0';
        result[1] = NULL;
        return result;
    }

    if (regcomp(&regex, "[A-Z]", REG_EXTENDED) != 0) {
        return NULL;
    }

    while (*p != '\0') {
        if (regexec(&regex, p, 1, &match, 0) == 0) {
            count++;
            p += match.rm_eo;
        } else {
            break;
        }
    }

    result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        regfree(&regex);
        return NULL;
    }

    p = input;
    start = 0;
    i = 0;

    while (i < count) {
        int found = 0;
        int end = len;
        const char *q = p;

        while (*q != '\0') {
            if (regexec(&regex, q, 1, &match, 0) == 0) {
                end = (q - input) + match.rm_eo;
                found = 1;
                break;
            }
            q++;
        }

        if (!found) {
            end = len;
        }

        if (i == count - 1) {
            end = len;
        }

        result[i] = malloc((end - start + 1) * sizeof(char));
        if (!result[i]) {
            int j;
            for (j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            regfree(&regex);
            return NULL;
        }

        strncpy(result[i], input + start, end - start);
        result[i][end - start] = '\0';

        start = end;
        p = input + start;
        i++;
    }

    result[count] = NULL;
    regfree(&regex);
    return result;
}

void free_split_result(char **result) {
    int i = 0;
    if (!result) return;
    while (result[i] != NULL) {
        free(result[i]);
        i++;
    }
    free(result);
}

int main(void) {
    const char *test_string = "helloWorldThisIsATest";
    char **parts = split_at_uppercase(test_string);
    int i = 0;

    if (!parts) {
        fprintf(stderr, "Failed to split string\n");
        return 1;
    }

    printf("Original: %s\n", test_string);
    printf("Split parts:\n");
    while (parts[i] != NULL) {
        printf("  [%d]: '%s'\n", i, parts[i]);
        i++;
    }

    free_split_result(parts);
    return 0;
}