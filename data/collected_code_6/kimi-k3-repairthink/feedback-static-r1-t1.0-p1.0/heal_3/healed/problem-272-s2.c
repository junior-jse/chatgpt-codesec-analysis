#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TUPLE_NAME_SIZE 32

typedef struct {
    int id;
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

int tuple_init(Tuple *tuple, int id, const char *name, double value) {
    size_t name_len;

    if (tuple == NULL || name == NULL) {
        return -1;
    }

    name_len = tuple_strnlen(name, TUPLE_NAME_SIZE);
    if (name_len >= TUPLE_NAME_SIZE) {
        return -2;
    }

    memset(tuple, 0, sizeof(*tuple));

    memcpy(tuple->name, name, name_len + 1);
    tuple->name[TUPLE_NAME_SIZE - 1] = '\0';

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

    if (memchr(tuple->name, '\0', sizeof(tuple->name)) == NULL) {
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

    if (list->head == NULL || list->tail == NULL || list->size == 0) {
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
    if (tuple == NULL) {
        return;
    }

    printf("id=%d, name=%.*s, value=%.2f\n",
           tuple->id,
           (int)tuple_strnlen(tuple->name, TUPLE_NAME_SIZE),
           tuple->name,
           tuple->value);
}

int main(void) {
    TupleList list;
    Tuple t1;
    Tuple t2;
    Tuple t3;
    Tuple extracted;
    int status;

    list_init(&list);
    memset(&extracted, 0, sizeof(extracted));

    if (tuple_init(&t1, 1, "record_one", 10.5) != 0) {
        fprintf(stderr, "Failed to initialize t1\n");
        return EXIT_FAILURE;
    }

    if (tuple_init(&t2, 2, "record_two", 20.75) != 0) {
        fprintf(stderr, "Failed to initialize t2\n");
        return EXIT_FAILURE;
    }

    if (tuple_init(&t3, 3, "record_three", 30.25) != 0) {
        fprintf(stderr, "Failed to initialize t3\n");
        return EXIT_FAILURE;
    }

    if (list_append(&list, &t1) != 0) {
        fprintf(stderr, "Failed to append t1\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }

    if (list_append(&list, &t2) != 0) {
        fprintf(stderr, "Failed to append t2\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }

    if (list_append(&list, &t3) != 0) {
        fprintf(stderr, "Failed to append t3\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }

    printf("List size before extraction: %zu\n", list.size);

    status = list_extract_rear(&list, &extracted);
    if (status == 0) {
        extracted.name[TUPLE_NAME_SIZE - 1] = '\0';
        printf("Extracted: ");
        tuple_print(&extracted);
        printf("List size after extraction: %zu\n", list.size);
    } else {
        fprintf(stderr, "Failed to extract rear element\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }

    status = list_extract_rear(&list, &extracted);
    if (status == 0) {
        extracted.name[TUPLE_NAME_SIZE - 1] = '\0';
        printf("Extracted: ");
        tuple_print(&extracted);
        printf("List size after extraction: %zu\n", list.size);
    } else {
        fprintf(stderr, "Failed to extract rear element\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }

    list_destroy(&list);

    return EXIT_SUCCESS;
}