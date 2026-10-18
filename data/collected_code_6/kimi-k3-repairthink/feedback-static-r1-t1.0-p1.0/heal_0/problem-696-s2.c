#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    size_t size;
} List;

typedef struct ZipNode {
    List *first;
    List *second;
    struct ZipNode *next;
} ZipNode;

typedef struct ZippedList {
    ZipNode *head;
    ZipNode *tail;
    size_t size;
} ZippedList;

static Node *create_node(int value)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

static List *create_list(void)
{
    List *list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

static int list_append(List *list, int value)
{
    Node *node;
    if (list == NULL) {
        return -1;
    }
    node = create_node(value);
    if (node == NULL) {
        return -1;
    }
    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
    return 0;
}

static void free_list(List *list)
{
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
    free(list);
}

static List *copy_list(const List *src)
{
    List *dst;
    const Node *current;
    if (src == NULL) {
        return NULL;
    }
    dst = create_list();
    if (dst == NULL) {
        return NULL;
    }
    current = src->head;
    while (current != NULL) {
        if (list_append(dst, current->value) != 0) {
            free_list(dst);
            return NULL;
        }
        current = current->next;
    }
    return dst;
}

static ZipNode *create_zip_node(const List *first, const List *second)
{
    ZipNode *node = malloc(sizeof(ZipNode));
    if (node == NULL) {
        return NULL;
    }
    node->first = copy_list(first);
    if (node->first == NULL) {
        free(node);
        return NULL;
    }
    node->second = copy_list(second);
    if (node->second == NULL) {
        free_list(node->first);
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

static ZippedList *create_zipped_list(void)
{
    ZippedList *zipped = malloc(sizeof(ZippedList));
    if (zipped == NULL) {
        return NULL;
    }
    zipped->head = NULL;
    zipped->tail = NULL;
    zipped->size = 0;
    return zipped;
}

static int zipped_append(ZippedList *zipped, const List *first, const List *second)
{
    ZipNode *node;
    if (zipped == NULL || first == NULL || second == NULL) {
        return -1;
    }
    node = create_zip_node(first, second);
    if (node == NULL) {
        return -1;
    }
    if (zipped->tail != NULL) {
        zipped->tail->next = node;
    } else {
        zipped->head = node;
    }
    zipped->tail = node;
    zipped->size++;
    return 0;
}

static void free_zipped_list(ZippedList *zipped)
{
    ZipNode *current;
    ZipNode *next;
    if (zipped == NULL) {
        return;
    }
    current = zipped->head;
    while (current != NULL) {
        next = current->next;
        free_list(current->first);
        free_list(current->second);
        free(current);
        current = next;
    }
    free(zipped);
}

ZippedList *zip_lists(const List *lists_a, size_t count_a, const List *lists_b, size_t count_b)
{
    ZippedList *result;
    size_t zip_count;
    size_t i;
    if (lists_a == NULL || lists_b == NULL) {
        return NULL;
    }
    result = create_zipped_list();
    if (result == NULL) {
        return NULL;
    }
    zip_count = (count_a < count_b) ? count_a : count_b;
    for (i = 0; i < zip_count; i++) {
        if (zipped_append(result, &lists_a[i], &lists_b[i]) != 0) {
            free_zipped_list(result);
            return NULL;
        }
    }
    return result;
}

static void print_list(const List *list)
{
    const Node *current;
    if (list == NULL) {
        printf("(null)");
        return;
    }
    printf("[");
    current = list->head;
    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }
    printf("]");
}

static void print_zipped(const ZippedList *zipped)
{
    const ZipNode *current;
    if (zipped == NULL) {
        printf("(null)\n");
        return;
    }
    printf("[");
    current = zipped->head;
    while (current != NULL) {
        printf("(");
        print_list(current->first);
        printf(", ");
        print_list(current->second);
        printf(")");
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

int main(void)
{
    List *lists_a = NULL;
    List *lists_b = NULL;
    ZippedList *zipped = NULL;
    size_t count_a = 3;
    size_t count_b = 3;
    size_t i;
    int status = EXIT_SUCCESS;

    lists_a = malloc(count_a * sizeof(List));
    if (lists_a == NULL) {
        status = EXIT_FAILURE;
        goto cleanup;
    }
    lists_b = malloc(count_b * sizeof(List));
    if (lists_b == NULL) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    for (i = 0; i < count_a; i++) {
        lists_a[i].head = NULL;
        lists_a[i].tail = NULL;
        lists_a[i].size = 0;
    }
    for (i = 0; i < count_b; i++) {
        lists_b[i].head = NULL;
        lists_b[i].tail = NULL;
        lists_b[i].size = 0;
    }

    if (list_append(&lists_a[0], 1) != 0 ||
        list_append(&lists_a[0], 2) != 0 ||
        list_append(&lists_a[1], 3) != 0 ||
        list_append(&lists_a[1], 4) != 0 ||
        list_append(&lists_a[2], 5) != 0 ||
        list_append(&lists_a[2], 6) != 0) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    if (list_append(&lists_b[0], 7) != 0 ||
        list_append(&lists_b[0], 8) != 0 ||
        list_append(&lists_b[1], 9) != 0 ||
        list_append(&lists_b[1], 10) != 0 ||
        list_append(&lists_b[2], 11) != 0 ||
        list_append(&lists_b[2], 12) != 0) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    zipped = zip_lists(lists_a, count_a, lists_b, count_b);
    if (zipped == NULL) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    print_zipped(zipped);

cleanup:
    free_zipped_list(zipped);
    if (lists_a != NULL) {
        for (i = 0; i < count_a; i++) {
            Node *current = lists_a[i].head;
            while (current != NULL) {
                Node *next = current->next;
                free(current);
                current = next;
            }
        }
        free(lists_a);
    }
    if (lists_b != NULL) {
        for (i = 0; i < count_b; i++) {
            Node *current = lists_b[i].head;
            while (current != NULL) {
                Node *next = current->next;
                free(current);
                current = next;
            }
        }
        free(lists_b);
    }
    return status;
}