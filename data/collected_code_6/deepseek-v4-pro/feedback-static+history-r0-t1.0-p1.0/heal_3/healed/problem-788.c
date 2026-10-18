#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        size_t str_len = strnlen(str, 4096);
        if (str_len == 4096 && str[4095] != '\0') {
            fprintf(stderr, "String too long or not null-terminated\n");
            exit(EXIT_FAILURE);
        }
        t.str = malloc(str_len + 1);
        if (t.str == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        if (str_len >= (size_t)-1 / 2) {
            free(t.str);
            t.str = NULL;
            fprintf(stderr, "Buffer overflow detected\n");
            exit(EXIT_FAILURE);
        }
        memcpy(t.str, str, str_len + 1);
    }

    t.list_len = list_len;

    if (list == NULL || list_len == 0) {
        t.list = NULL;
    } else {
        if (list_len > (size_t)-1 / sizeof(int)) {
            if (t.str != NULL) {
                free(t.str);
                t.str = NULL;
            }
            fprintf(stderr, "Integer overflow detected\n");
            exit(EXIT_FAILURE);
        }
        size_t alloc_size = list_len * sizeof(int);
        t.list = malloc(alloc_size);
        if (t.list == NULL) {
            free(t.str);
            t.str = NULL;
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        memcpy(t.list, list, alloc_size);
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