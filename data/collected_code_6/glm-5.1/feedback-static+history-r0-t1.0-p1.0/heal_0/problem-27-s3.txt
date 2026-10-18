#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_digits(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    char *shrunk = realloc(result, j + 1);
    return shrunk != NULL ? shrunk : result;
}

void remove_digits_from_list(char **list, size_t size) {
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            char *new_str = remove_digits(list[i]);
            if (new_str != NULL) {
                free(list[i]);
                list[i] = new_str;
            }
        }
    }
}

int main(void) {
    char *list[] = {
        strdup("app1le"),
        strdup("b2an3a4na"),
        strdup("5ch6er7y"),
        strdup("o8ra9nge"),
        strdup("no digits"),
        strdup("12345"),
        strdup("")
    };

    size_t size = sizeof(list) / sizeof(list[0]);

    remove_digits_from_list(list, size);

    for (size_t i = 0; i < size; i++) {
        printf("%s\n", list[i]);
        free(list[i]);
    }

    return 0;
}