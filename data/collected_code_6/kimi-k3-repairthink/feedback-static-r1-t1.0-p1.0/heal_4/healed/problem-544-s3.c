#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_ITEM_LEN 4096

typedef struct {
    char **items;
    size_t count;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleList;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *copy_string(const char *src, size_t max_len) {
    if (src == NULL) {
        return NULL;
    }
    size_t len = bounded_strlen(src, max_len);
    if (len >= SIZE_MAX) {
        return NULL;
    }
    size_t dest_size = len + 1;
    char *dest = malloc(dest_size);
    if (dest == NULL) {
        return NULL;
    }
    size_t k;
    for (k = 0; k < len && k + 1 < dest_size; k++) {
        dest[k] = src[k];
    }
    dest[k] = '\0';
    return dest;
}

char *flatten_tuple_list(const TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return NULL;
    }

    size_t total_len = 0;
    size_t i, j;

    for (i = 0; i < list->count; i++) {
        if (list->tuples[i].items == NULL) {
            continue;
        }
        for (j = 0; j < list->tuples[i].count; j++) {
            if (list->tuples[i].items[j] != NULL) {
                size_t len = bounded_strlen(list->tuples[i].items[j], MAX_ITEM_LEN);
                if (len >= SIZE_MAX - total_len - 1) {
                    return NULL;
                }
                total_len += len + 1;
            }
        }
    }

    if (total_len >= SIZE_MAX) {
        return NULL;
    }

    size_t capacity = total_len + 1;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;

    for (i = 0; i < list->count; i++) {
        if (list->tuples[i].items == NULL) {
            continue;
        }
        for (j = 0; j < list->tuples[i].count; j++) {
            if (list->tuples[i].items[j] != NULL) {
                size_t len = bounded_strlen(list->tuples[i].items[j], MAX_ITEM_LEN);
                if (pos >= capacity || len + 1 > capacity - pos) {
                    goto finish;
                }
                size_t k;
                for (k = 0; k < len && pos + 1 < capacity; k++) {
                    result[pos] = list->tuples[i].items[j][k];
                    pos++;
                }
                if (pos < capacity) {
                    result[pos] = ' ';
                    pos++;
                }
            }
        }
    }

finish:
    if (pos > 0) {
        result[pos - 1] = '\0';
    } else {
        result[0] = '\0';
    }

    return result;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }

    size_t i, j;
    for (i = 0; i < list->count; i++) {
        if (list->tuples[i].items != NULL) {
            for (j = 0; j < list->tuples[i].count; j++) {
                free(list->tuples[i].items[j]);
            }
            free(list->tuples[i].items);
            list->tuples[i].items = NULL;
        }
        list->tuples[i].count = 0;
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

int main(void) {
    TupleList list;
    list.tuples = NULL;
    list.count = 0;

    list.tuples = calloc(2, sizeof(Tuple));
    if (list.tuples == NULL) {
        return EXIT_FAILURE;
    }
    list.count = 2;

    list.tuples[0].items = calloc(2, sizeof(char *));
    if (list.tuples[0].items == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }
    list.tuples[0].count = 2;

    list.tuples[0].items[0] = copy_string("Hello", MAX_ITEM_LEN);
    if (list.tuples[0].items[0] == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    list.tuples[0].items[1] = copy_string("World", MAX_ITEM_LEN);
    if (list.tuples[0].items[1] == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    list.tuples[1].items = calloc(3, sizeof(char *));
    if (list.tuples[1].items == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }
    list.tuples[1].count = 3;

    list.tuples[1].items[0] = copy_string("C", MAX_ITEM_LEN);
    if (list.tuples[1].items[0] == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    list.tuples[1].items[1] = copy_string("Programming", MAX_ITEM_LEN);
    if (list.tuples[1].items[1] == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    list.tuples[1].items[2] = copy_string("Language", MAX_ITEM_LEN);
    if (list.tuples[1].items[2] == NULL) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    char *flattened = flatten_tuple_list(&list);
    if (flattened != NULL) {
        printf("%s\n", flattened);
        free(flattened);
    }

    free_tuple_list(&list);

    return EXIT_SUCCESS;
}