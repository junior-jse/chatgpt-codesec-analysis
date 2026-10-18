#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *string;
    int *list;
    size_t list_len;
} Tuple;

Tuple *create_tuple(const char *str, const int *list, size_t list_len) {
    if (str == NULL || (list == NULL && list_len > 0)) {
        return NULL;
    }

    Tuple *t = (Tuple *)malloc(sizeof(Tuple));
    if (t == NULL) {
        return NULL;
    }

    t->string = (char *)malloc(strlen(str) + 1);
    if (t->string == NULL) {
        free(t);
        return NULL;
    }
    strcpy(t->string, str);

    t->list_len = list_len;
    if (list_len > 0) {
        t->list = (int *)malloc(list_len * sizeof(int));
        if (t->list == NULL) {
            free(t->string);
            free(t);
            return NULL;
        }
        memcpy(t->list, list, list_len * sizeof(int));
    } else {
        t->list = NULL;
    }

    return t;
}

void free_tuple(Tuple *t) {
    if (t != NULL) {
        free(t->string);
        free(t->list);
        free(t);
    }
}

int main(void) {
    const char *str = "example";
    int list[] = {1, 2, 3, 4, 5};
    size_t list_len = sizeof(list) / sizeof(list[0]);

    Tuple *t = create_tuple(str, list, list_len);
    if (t == NULL) {
        return EXIT_FAILURE;
    }

    printf("String: %s\n", t->string);
    printf("List: ");
    for (size_t i = 0; i < t->list_len; i++) {
        printf("%d ", t->list[i]);
    }
    printf("\n");

    free_tuple(t);

    return EXIT_SUCCESS;
}