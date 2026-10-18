#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 4096
#define BUF_SIZE 32

char *remove_digits(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (str[len] != '\0' && len < MAX_STR_LEN) {
        len++;
    }
    if (len == MAX_STR_LEN) {
        return NULL;
    }

    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (!isdigit(c)) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

int remove_all_digits(char **list, size_t size) {
    if (list == NULL) {
        return -1;
    }

    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }

        char *filtered = remove_digits(list[i]);
        if (filtered == NULL) {
            return -1;
        }

        free(list[i]);
        list[i] = filtered;
    }

    return 0;
}

int main(void) {
    size_t count = 4;
    char **list = malloc(count * sizeof(char *));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        list[i] = malloc(BUF_SIZE);
        if (list[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return EXIT_FAILURE;
        }
    }

    snprintf(list[0], BUF_SIZE, "%s", "h3ll0");
    snprintf(list[1], BUF_SIZE, "%s", "w0rld123");
    snprintf(list[2], BUF_SIZE, "%s", "c0d1ng");
    snprintf(list[3], BUF_SIZE, "%s", "t3st456");

    if (remove_all_digits(list, count) != 0) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    for (size_t i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);

    return EXIT_SUCCESS;
}