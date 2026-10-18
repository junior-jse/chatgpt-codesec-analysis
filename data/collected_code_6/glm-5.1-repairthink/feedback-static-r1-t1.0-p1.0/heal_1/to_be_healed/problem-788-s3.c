#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *string;
    int *list;
    size_t list_size;
} Tuple;

Tuple *create_tuple(const char *str, const int *list, size_t list_size) {
    if (str == NULL || (list == NULL && list_size > 0)) {
        return NULL;
    }

    Tuple *t = (Tuple *)malloc(sizeof(Tuple));
    if (t == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    t->string = (char *)malloc(strlen(str) + 1);
    if (t->string == NULL) {
        free(t);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(t->string, str);

    if (list_size > 0) {
        t->list = (int *)malloc(list_size * sizeof(int));
        if (t->list == NULL) {
            free(t->string);
            free(t);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(t->list, list, list_size * sizeof(int));
    } else {
        t->list = NULL;
    }

    t->list_size = list_size;
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
    int arr[] = {1, 2, 3, 4, 5};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);

    Tuple *t = create_tuple(str, arr, arr_size);
    if (t == NULL) {
        return EXIT_FAILURE;
    }

    printf("String: %s\nList: [", t->string);
    for (size_t i = 0; i < t->list_size; ++i) {
        printf("%d%s", t->list[i], i < t->list_size - 1 ? ", " : "");
    }
    printf("]\n");

    free_tuple(t);

    return EXIT_SUCCESS;
}