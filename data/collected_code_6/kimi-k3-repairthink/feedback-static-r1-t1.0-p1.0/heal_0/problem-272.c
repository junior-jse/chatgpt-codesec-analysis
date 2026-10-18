#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[32];
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

void list_init(TupleList *list)
{
    if (list != NULL) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
}

int list_append(TupleList *list, const Tuple *data)
{
    Node *new_node;
    
    if (list == NULL || data == NULL) {
        return -1;
    }
    
    new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return -1;
    }
    
    memcpy(&new_node->data, data, sizeof(*data));
    new_node->next = NULL;
    
    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    
    list->tail = new_node;
    list->size++;
    
    return 0;
}

int list_extract_rear(TupleList *list, Tuple *out)
{
    Node *current;
    Node *prev;
    
    if (list == NULL || out == NULL) {
        return -1;
    }
    
    if (list->tail == NULL) {
        return -1;
    }
    
    memcpy(out, &list->tail->data, sizeof(*out));
    
    current = list->head;
    prev = NULL;
    
    while (current != NULL && current->next != NULL) {
        prev = current;
        current = current->next;
    }
    
    if (prev != NULL) {
        prev->next = NULL;
        list->tail = prev;
    } else {
        list->head = NULL;
        list->tail = NULL;
    }
    
    free(current);
    list->size--;
    
    return 0;
}

void list_destroy(TupleList *list)
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
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int main(void)
{
    TupleList list;
    Tuple extracted;
    Tuple t1 = {1, "record_a", 10.5};
    Tuple t2 = {2, "record_b", 20.75};
    Tuple t3 = {3, "record_c", 30.25};
    int status;
    
    list_init(&list);
    
    status = list_append(&list, &t1);
    if (status != 0) {
        fprintf(stderr, "Failed to append t1\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }
    
    status = list_append(&list, &t2);
    if (status != 0) {
        fprintf(stderr, "Failed to append t2\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }
    
    status = list_append(&list, &t3);
    if (status != 0) {
        fprintf(stderr, "Failed to append t3\n");
        list_destroy(&list);
        return EXIT_FAILURE;
    }
    
    printf("List size before extraction: %zu\n", list.size);
    
    status = list_extract_rear(&list, &extracted);
    if (status == 0) {
        printf("Extracted: id=%d, name=%s, value=%.2f\n", 
               extracted.id, extracted.name, extracted.value);
        printf("List size after extraction: %zu\n", list.size);
    } else {
        fprintf(stderr, "Failed to extract rear element\n");
    }
    
    status = list_extract_rear(&list, &extracted);
    if (status == 0) {
        printf("Extracted: id=%d, name=%s, value=%.2f\n", 
               extracted.id, extracted.name, extracted.value);
    }
    
    list_destroy(&list);
    
    return EXIT_SUCCESS;
}