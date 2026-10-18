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
    /* Possible weaknesses found:
     *  dest_size is assigned 'len+1' here.
     */
    size_t dest_size = len + 1;
    char *dest = malloc(dest_size);
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (dest == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     *  Condition 'len>=dest_size' is always false
     *  Condition 'len>=dest_size' is always false [knownConditionTrueFalse]
     */
    if (len >= dest_size) {
        free(dest);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest, src, len);
    dest[len] = '\0';
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
                if (len > MAX_ITEM_LEN || len + 1 > SIZE_MAX - total_len) {
                    return NULL;
                }
                total_len += len + 1;
            }
        }
    }

    if (total_len == 0) {
        char *empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
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
                    break;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + pos, list->tuples[i].items[j], len);
                pos += len;
                result[pos++] = ' ';
            }
        }
    }

    if (pos > 0) {
        result[pos - 1] = '\0';
    } else {
        result[pos] = '\0';
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