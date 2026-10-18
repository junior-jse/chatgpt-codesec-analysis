#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Tuple {
    char *str;
    int *list;
    size_t list_len;
} Tuple;

Tuple create_tuple(const char *str, size_t str_len, const int *list, size_t list_len) {
    Tuple t;

    if (str == NULL) {
        t.str = NULL;
    } else {
        if (str_len > SIZE_MAX - 1) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        t.str = malloc(str_len + 1);
        if (t.str == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        memcpy(t.str, str, str_len);
        t.str[str_len] = '\0';
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

    t = create_tuple(str, 5, arr, 5);

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