#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_parenthesis(const char *str, size_t len) {
    if (str == NULL) {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    int depth = 0;

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '(') {
            depth++;
        } else if (str[i] == ')') {
            if (depth > 0) {
                depth--;
            } else {
                result[j++] = str[i];
            }
        } else {
            if (depth == 0) {
                result[j++] = str[i];
            }
        }
    }

    result[j] = '\0';

    char *shrunk = (char *)realloc(result, j + 1);
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    const char *input1 = "Hello (World) this (is a) test!";
    char *output1 = remove_parenthesis(input1, strnlen(input1, 1024));
    if (output1 != NULL) {
        printf("%s\n", output1);
        free(output1);
    }

    const char *input2 = "(Nested (Parentheses)) Outside";
    char *output2 = remove_parenthesis(input2, strnlen(input2, 1024));
    if (output2 != NULL) {
        printf("%s\n", output2);
        free(output2);
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    const char buf[6] = {'H', 'e', 'l', 'l', 'o', '('};
    char *output3 = remove_parenthesis(buf, 6);
    if (output3 != NULL) {
        for (size_t i = 0; i < 5; i++) {
            putchar(output3[i]);
        }
        putchar('\n');
        free(output3);
    }

    return 0;
}