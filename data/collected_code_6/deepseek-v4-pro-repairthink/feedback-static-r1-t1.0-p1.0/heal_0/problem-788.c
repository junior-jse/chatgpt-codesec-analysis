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
    size_t i;

    if (str == NULL) {
        t.str = NULL;
    } else {
        t.str = malloc(strlen(str) + 1);
        if (t.str == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(t.str, str);
    }

    t.list_len = list_len;

    if (list == NULL || list_len == 0) {
        t.list = NULL;
    } else {
        t.list = malloc(list_len * sizeof(int));
        if (t.list == NULL) {
            free(t.str);
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < list_len; i++) {
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
    int arr[] = {1, 2, 3, 4, 5};
    Tuple t;
    size_t i;

    t = create_tuple(str, arr, 5);

    if (t.str != NULL) {
        printf("String: %s\n", t.str);
    }

    if (t.list != NULL) {
        printf("List: ");
        for (i = 0; i < t.list_len; i++) {
            printf("%d ", t.list[i]);
        }
        printf("\n");
    }

    free_tuple(&t);

    return 0;
}