#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TUPLE_NAME_SIZE 32

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char name[TUPLE_NAME_SIZE];
    double value;
} Tuple;

typedef struct Node {
    Tuple data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} TupleList;

static size_t tuple_strnlen(const char *s, size_t max_len) {
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

static int tuple_copy_name(char *dest, size_t dest_size, const char *src) {
    size_t src_len;

    if (dest == NULL || dest_size == 0U || src == NULL) {
        return -1;
    }

    src_len = tuple_strnlen(src, dest_size);
    if (src_len >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest, src, src_len);
    dest[src_len] = '\0';

    return 0;
}

int tuple_init(Tuple *tuple, int id, const char *name, double value) {
    if (tuple == NULL || name == NULL) {
        return -1;
    }

    memset(tuple, 0, sizeof(*tuple));

    if (tuple_copy_name(tuple->name, sizeof(tuple->name), name) != 0) {
        return -2;
    }

    tuple->id = id;
    tuple->value = value;

    return 0;
}

void list_init(TupleList *list) {
    if (list == NULL) {
        return;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int list_append(TupleList *list, const Tuple *tuple) {
    Node *new_node;

    if (list == NULL || tuple == NULL) {
        return -1;
    }

    if (tuple_strnlen(tuple->name, sizeof(tuple->name)) >= sizeof(tuple->name)) {
        return -1;
    }

    new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return -1;
    }

    new_node->data = *tuple;
    new_node->data.name[TUPLE_NAME_SIZE - 1] = '\0';
    new_node->next = NULL;

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;

    return 0;
}

int list_extract_rear(TupleList *list, Tuple *out_tuple) {
    Node *current;
    Node *prev;

    if (list == NULL || out_tuple == NULL) {
        return -1;
    }

    if (list->head == NULL || list->tail == NULL || list->size == 0U) {
        return -1;
    }

    *out_tuple = list->tail->data;
    out_tuple->name[TUPLE_NAME_SIZE - 1] = '\0';

    if (list->head == list->tail) {
        free(list->tail);
        list->head = NULL;
        list->tail = NULL;
    } else {
        prev = NULL;
        current = list->head;

        while (current != NULL && current->next != NULL) {
            prev = current;
            current = current->next;
        }

        if (prev == NULL || current == NULL) {
            return -1;
        }

        free(current);
        prev->next = NULL;
        list->tail = prev;
    }

    list->size--;

    return 0;
}

void list_destroy(TupleList *list) {
    Node *current;
    Node *next;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

static void tuple_print(const Tuple *tuple) {
    int name_len;

    if (tuple == NULL) {
        return;
    }

    name_len = (int)tuple_strnlen(tuple->name, TUPLE_NAME_SIZE);
    printf("id=%d, name=%.*s, value=%.2f\n",
           tuple->id, name_len, tuple->name, tuple->value);
}

int main(void) {
    TupleList list;
    Tuple tuples[3];
    Tuple extracted;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    static const char *names[3] = {"record_one", "record_two", "record_three"};
    static const double values[3] = {10.5, 20.75, 30.25};

    list_init(&list);
    memset(&extracted, 0, sizeof(extracted));

    for (size_t i = 0; i < 3; i++) {
        if (tuple_init(&tuples[i], (int)i + 1, names[i], values[i]) != 0) {
            fprintf(stderr, "Failed to initialize tuple %zu\n", i + 1);
            return EXIT_FAILURE;
        }

        if (list_append(&list, &tuples[i]) != 0) {
            fprintf(stderr, "Failed to append tuple %zu\n", i + 1);
            list_destroy(&list);
            return EXIT_FAILURE;
        }
    }

    printf("List size before extraction: %zu\n", list.size);

    for (size_t i = 0; i < 2; i++) {
        int status = list_extract_rear(&list, &extracted);
        if (status != 0) {
            fprintf(stderr, "Failed to extract rear element\n");
            list_destroy(&list);
            return EXIT_FAILURE;
        }

        extracted.name[TUPLE_NAME_SIZE - 1] = '\0';
        printf("Extracted: ");
        tuple_print(&extracted);
        printf("List size after extraction: %zu\n", list.size);
    }

    list_destroy(&list);

    return EXIT_SUCCESS;
}