#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *snake_to_camel(const char *snake) {
    size_t len;
    size_t i;
    size_t j;
    char *camel;

    if (snake == NULL) {
        return NULL;
    }

    len = strlen(snake);
    camel = (char *)malloc(len + 1);
    if (camel == NULL) {
        return NULL;
    }

    i = 0;
    j = 0;

    while (i < len && snake[i] == '_') {
        i++;
    }

    while (i < len) {
        if (snake[i] == '_') {
            i++;
            while (i < len && snake[i] == '_') {
                i++;
            }
            if (i < len) {
                camel[j++] = (char)((unsigned char)snake[i] >= 'a' && (unsigned char)snake[i] <= 'z' ? snake[i] - 32 : snake[i]);
            }
        } else {
            camel[j++] = snake[i];
        }
        i++;
    }

    camel[j] = '\0';

    return camel;
}

int main(void) {
    char *result;

    result = snake_to_camel("hello_world_example");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = snake_to_camel("__leading_underscore");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = snake_to_camel("trailing_underscore__");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = snake_to_camel("multiple___underscores");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    return 0;
}