#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LEN 4096

static size_t safe_strnlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i] != '\0') {
        i++;
    }
    return i;
}

typedef struct Tuple {
    char *str;
    int *list;
    size_t list_len;
} Tuple;

Tuple create_tuple(const char *str, const int *list, size_t list_len) {
    Tuple t;

    if (str == NULL) {
        t.str = NULL;
    } else {
        size_t len = safe_strnlen(str, MAX_STRING_LEN);
        if (len == MAX_STRING_LEN) {
            fprintf(stderr, "String too long or not null-terminated\n");
            exit(EXIT_FAILURE);
        }
        t.str = malloc(len + 1);
        if (t.str == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        memcpy(t.str, str, len);
        t.str[len] = '\0';
    }

    t.list_len = list_len;

    if (list == NULL || list_len == 0) {
        t.list = NULL;
    } else {
        if (list_len > SIZE_MAX / sizeof(int)) {
            free(t.str);
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        t.list = malloc(list_len * sizeof(int));
        if (t.list == NULL) {
            free(t.str);
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < list_len; i++) {
            t.list[i] = list[i];
        }
    }

    return t;
}

void free_tuple(Tuple *t) {
    if (t == NULL) return;
    free(t->str);
    free(t->list);
    t->str = NULL;
    t->list = NULL;
    t->list_len = 0;
}

int main(void) {
    const char *str = "hello";
    const int arr[] = {1, 2, 3, 4, 5};
    Tuple t;

    t = create_tuple(str, arr, 5);

    if (t.str != NULL) {
        printf("String: %s\n", t.str);
    }

    if (t.list != NULL) {
        printf("List: ");
        for (size_t i = 0; i < t.list_len; i++) {
            printf("%d ", t.list[i]);
        }
        printf("\n");
    }

    free_tuple(&t);

    return 0;
}