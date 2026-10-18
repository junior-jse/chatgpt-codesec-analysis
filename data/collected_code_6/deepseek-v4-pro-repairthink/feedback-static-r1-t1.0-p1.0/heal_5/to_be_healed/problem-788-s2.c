#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int *list;
    int list_size;
} Tuple;

Tuple create_tuple(const char *str, size_t str_len, const int *list, int list_size) {
    Tuple t;
    t.str = NULL;
    t.list = NULL;
    t.list_size = 0;

    if (str != NULL && str_len > 0) {
        t.str = malloc(str_len + 1);
        if (t.str == NULL) {
            fprintf(stderr, "Memory allocation failed for string\n");
            exit(EXIT_FAILURE);
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(t.str, str, str_len);
        t.str[str_len] = '\0';
    }

    if (list_size > 0) {
        if (list == NULL) {
            fprintf(stderr, "Invalid arguments: null list with positive size\n");
            exit(EXIT_FAILURE);
        }
        t.list = malloc(sizeof(int) * list_size);
        if (t.list == NULL) {
            free(t.str);
            t.str = NULL;
            fprintf(stderr, "Memory allocation failed for list\n");
            exit(EXIT_FAILURE);
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(t.list, list, sizeof(int) * list_size);
        t.list_size = list_size;
    }

    return t;
}

void free_tuple(Tuple *t) {
    if (t != NULL) {
        free(t->str);
        free(t->list);
        t->str = NULL;
        t->list = NULL;
        t->list_size = 0;
    }
}

int main(void) {
    const char str[] = "hello";
    int list_data[] = {1, 2, 3, 4, 5};
    int list_size = sizeof(list_data) / sizeof(list_data[0]);

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    Tuple t = create_tuple(str, strlen(str), list_data, list_size);

    printf("String: %s\n", t.str ? t.str : "(null)");
    printf("List: ");
    if (t.list != NULL) {
        for (int i = 0; i < t.list_size; i++) {
            printf("%d ", t.list[i]);
        }
    } else {
        printf("(empty)");
    }
    printf("\n");

    free_tuple(&t);

    return 0;
}