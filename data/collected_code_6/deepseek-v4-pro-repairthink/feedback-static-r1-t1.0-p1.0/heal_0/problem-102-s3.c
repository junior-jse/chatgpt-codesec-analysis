#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *snake_to_camel(const char *snake) {
    if (snake == NULL) {
        return NULL;
    }

    size_t len = strlen(snake);
    char *camel = (char *)malloc(len + 1);
    if (camel == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    int capitalize_next = 0;

    if (snake[0] == '_') {
        camel[j++] = '_';
        i++;
    }

    for (; i < len; i++) {
        if (snake[i] == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                camel[j++] = toupper((unsigned char)snake[i]);
                capitalize_next = 0;
            } else {
                camel[j++] = snake[i];
            }
        }
    }

    camel[j] = '\0';
    return camel;
}

int main(void) {
    const char *test1 = "snake_case_string";
    const char *test2 = "_leading_underscore";
    const char *test3 = "trailing_underscore_";
    const char *test4 = "multiple__underscores";
    const char *test5 = "alreadyCamel";
    const char *test6 = "";
    const char *test7 = "a";
    const char *test8 = "a_b_c_d_e_f";

    char *result = NULL;

    result = snake_to_camel(test1);
    if (result != NULL) {
        printf("%s -> %s\n", test1, result);
        free(result);
    }

    result = snake_to_camel(test2);
    if (result != NULL) {
        printf("%s -> %s\n", test2, result);
        free(result);
    }

    result = snake_to_camel(test3);
    if (result != NULL) {
        printf("%s -> %s\n", test3, result);
        free(result);
    }

    result = snake_to_camel(test4);
    if (result != NULL) {
        printf("%s -> %s\n", test4, result);
        free(result);
    }

    result = snake_to_camel(test5);
    if (result != NULL) {
        printf("%s -> %s\n", test5, result);
        free(result);
    }

    result = snake_to_camel(test6);
    if (result != NULL) {
        printf("\"%s\" -> \"%s\"\n", test6, result);
        free(result);
    }

    result = snake_to_camel(test7);
    if (result != NULL) {
        printf("%s -> %s\n", test7, result);
        free(result);
    }

    result = snake_to_camel(test8);
    if (result != NULL) {
        printf("%s -> %s\n", test8, result);
        free(result);
    }

    result = snake_to_camel(NULL);
    if (result != NULL) {
        printf("NULL -> %s\n", result);
        free(result);
    } else {
        printf("NULL -> NULL\n");
    }

    return 0;
}