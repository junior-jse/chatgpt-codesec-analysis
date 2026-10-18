#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_parenthesis_area(const char *str)
{
    char *result;
    size_t len;
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

    j = 0;
    depth = 0;

    for (i = 0; i < len; i++) {
        if (str[i] == '(') {
            depth++;
        } else if (str[i] == ')') {
            if (depth > 0) {
                depth--;
            }
        } else if (depth == 0) {
            result[j] = str[i];
            j++;
        }
    }

    result[j] = '\0';
    return result;
}

int main(void)
{
    char *output;
    const char *input = "Hello (this should be removed) World (this too)!";

    output = remove_parenthesis_area(input);
    if (output == NULL) {
        fprintf(stderr, "Error: Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", input);
    printf("Modified: %s\n", output);

    free(output);
    output = NULL;

    return EXIT_SUCCESS;
}