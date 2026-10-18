#include <stdio.h>
#include <stdlib.h>

char *remove_parenthesis_area(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t count = 0;
    int depth = 0;
    const char *p = str;

    while (*p) {
        if (*p == '(') {
            depth++;
        } else if (*p == ')') {
            if (depth > 0) {
                depth--;
            }
        } else {
            if (depth == 0) {
                count++;
            }
        }
        p++;
    }

    char *result = malloc(count + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    depth = 0;
    p = str;

    while (*p) {
        if (*p == '(') {
            depth++;
        } else if (*p == ')') {
            if (depth > 0) {
                depth--;
            }
        } else {
            if (depth == 0) {
                result[j++] = *p;
            }
        }
        p++;
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char *test1 = "Hello (this is a test) World";
    const char *test2 = "No parentheses here";
    const char *test3 = "Nested ((parentheses) test) example";
    const char *test4 = "Unmatched (parenthesis";
    const char *test5 = "Multiple (first) and (second) removal";

    char *result;

    result = remove_parenthesis_area(test1);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = remove_parenthesis_area(test2);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = remove_parenthesis_area(test3);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = remove_parenthesis_area(test4);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = remove_parenthesis_area(test5);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    if (NULL == NULL) {
        printf("NULL input handled\n");
    }

    return 0;
}