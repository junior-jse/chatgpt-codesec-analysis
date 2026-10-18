#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TUPLE_MAX_STR_LEN 4096

typedef struct {
    char *str;
    int *list;
    size_t list_len;
} Tuple;

Tuple *create_tuple(const char *str, const int *list, size_t list_len)
{
    Tuple *t;
    char *new_str;
    int *new_list;
    size_t str_len;
    size_t str_buf_size;
    size_t list_buf_size;
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;
    int written;

    if (str == NULL || (list == NULL && list_len > 0)) {
        return NULL;
    }

    str_len = strnlen(str, TUPLE_MAX_STR_LEN);
    if (str_len == TUPLE_MAX_STR_LEN) {
        return NULL;
    }

    if (list_len > SIZE_MAX / sizeof(int)) {
        return NULL;
    }

    str_buf_size = str_len + 1;
    list_buf_size = list_len * sizeof(int);

    t = (Tuple *)malloc(sizeof(Tuple));
    if (t == NULL) {
        return NULL;
    }

    new_str = (char *)malloc(str_buf_size);
    if (new_str == NULL) {
        free(t);
        return NULL;
    }

    written = snprintf(new_str, str_buf_size, "%s", str);
    if (written < 0 || (size_t)written >= str_buf_size) {
        free(new_str);
        free(t);
        return NULL;
    }

    new_list = NULL;
    if (list_len > 0) {
        new_list = (int *)malloc(list_buf_size);
        if (new_list == NULL) {
            free(new_str);
            free(t);
            return NULL;
        }
        for (i = 0; i < list_len && i < list_buf_size / sizeof(int); i++) {
            new_list[i] = list[i];
        }
    }

    t->str = new_str;
    t->list = new_list;
    t->list_len = list_len;

    return t;
}

void free_tuple(Tuple *t)
{
    if (t != NULL) {
        free(t->str);
        free(t->list);
        free(t);
    }
}

int main(void)
{
    const char *str = "example";
    int list[] = {1, 2, 3, 4, 5};
    size_t list_len = sizeof(list) / sizeof(list[0]);
    Tuple *t;
    size_t i;

    t = create_tuple(str, list, list_len);
    if (t == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return EXIT_FAILURE;
    }

    printf("String: %s\n", t->str);
    printf("List: ");
    for (i = 0; i < t->list_len; i++) {
        printf("%d ", t->list[i]);
    }
    printf("\n");

    free_tuple(t);

    return EXIT_SUCCESS;
}