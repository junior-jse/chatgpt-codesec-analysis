#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_parenthesis_area(const char *str)
{
    size_t len;
    char *result;
    size_t i;
    size_t j;
    int depth;

    if (str == NULL) {
        return NULL;
    }

    len = strlen(str);
    result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    i = 0;
    j = 0;
    depth = 0;

    while (str[i] != '\0') {
        if (str[i] == '(') {
            depth++;
        } else if (str[i] == ')') {
            if (depth > 0) {
                depth--;
            } else {
                result[j] = str[i];
                j++;
            }
        } else if (depth == 0) {
            result[j] = str[i];
            j++;
        }
        i++;
    }

    result[j] = '\0';

    return result;
}

int main(void)
{
    const char *test_str = "Hello (World) Test (Example) Done";
    char *result;

    result = remove_parenthesis_area(test_str);

    if (result != NULL) {
        printf("Original: %s\n", test_str);
        printf("Modified: %s\n", result);
        free(result);
    } else {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    return 0;
}