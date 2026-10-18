#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static size_t safe_strlen(const char *s) {
    size_t n = 0;
    while (n < SIZE_MAX && s[n] != '\0') {
        n++;
    }
    return n;
}

static char *reverse_string(const char *str, size_t len) {
    char *reversed = malloc(len + 1);
    if (!reversed) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        reversed[i] = str[len - 1 - i];
    }
    reversed[len] = '\0';
    return reversed;
}

int count_reverse_pairs(char **list, size_t size) {
    if (!list || size == 0) {
        return 0;
    }
    char **reversed_list = malloc(sizeof(char *) * size);
    if (!reversed_list) {
        return 0;
    }
    for (size_t i = 0; i < size; i++) {
        size_t len = safe_strlen(list[i]);
        reversed_list[i] = reverse_string(list[i], len);
        if (!reversed_list[i]) {
            for (size_t j = 0; j < i; j++) {
                free(reversed_list[j]);
            }
            free(reversed_list);
            return 0;
        }
    }
    int *used = calloc(size, sizeof(int));
    if (!used) {
        for (size_t i = 0; i < size; i++) {
            free(reversed_list[i]);
        }
        free(reversed_list);
        return 0;
    }
    int count = 0;
    for (size_t i = 0; i < size; i++) {
        if (used[i]) {
            continue;
        }
        for (size_t j = i + 1; j < size; j++) {
            if (!used[j] && strcmp(list[i], reversed_list[j]) == 0) {
                count++;
                used[i] = 1;
                used[j] = 1;
                break;
            }
        }
    }
    for (size_t i = 0; i < size; i++) {
        free(reversed_list[i]);
    }
    free(reversed_list);
    free(used);
    return count;
}

int main(void) {
    const char *words[] = {
        "abc", "cba", "test", "tset", "hello", "olleh", "world"
    };
    size_t size = sizeof(words) / sizeof(words[0]);
    char **list = malloc(sizeof(char *) * size);
    if (!list) {
        return 1;
    }
    for (size_t i = 0; i < size; i++) {
        size_t len = safe_strlen(words[i]);
        list[i] = malloc(len + 1);
        if (!list[i]) {
            for (size_t j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return 1;
        }
        snprintf(list[i], len + 1, "%s", words[i]);
    }
    int pairs = count_reverse_pairs(list, size);
    printf("Number of reverse pairs: %d\n", pairs);
    for (size_t i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
    return 0;
}