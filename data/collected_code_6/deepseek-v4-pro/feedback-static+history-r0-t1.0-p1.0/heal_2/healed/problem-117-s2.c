#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    void *data;
    int type;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} List;

#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRING 3

Node *create_node(void *data, int type) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->type = type;
    node->next = NULL;
    return node;
}

void list_append(List *list, void *data, int type) {
    Node *node = create_node(data, type);
    if (!node) return;
    if (!list->head) {
        list->head = node;
    } else {
        Node *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
}

void list_free(List *list) {
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
}

int is_valid_float_string(const char *str) {
    if (!str || !*str) return 0;
    char *endptr = NULL;
    strtod(str, &endptr);
    if (endptr == str) return 0;
    while (*endptr) {
        if (!isspace((unsigned char)*endptr)) return 0;
        endptr++;
    }
    return 1;
}

void convert_all_to_float(List *list) {
    Node *current = list->head;
    while (current) {
        if (current->type == TYPE_INT) {
            int *int_val = (int *)current->data;
            float *float_val = (float *)malloc(sizeof(float));
            if (!float_val) return;
            *float_val = (float)(*int_val);
            free(current->data);
            current->data = float_val;
            current->type = TYPE_FLOAT;
        } else if (current->type == TYPE_STRING) {
            char *str_val = (char *)current->data;
            if (is_valid_float_string(str_val)) {
                float *float_val = (float *)malloc(sizeof(float));
                if (!float_val) return;
                *float_val = strtof(str_val, NULL);
                free(current->data);
                current->data = float_val;
                current->type = TYPE_FLOAT;
            }
        }
        current = current->next;
    }
}

void print_list(List *list) {
    Node *current = list->head;
    while (current) {
        if (current->type == TYPE_INT) {
            printf("%d ", *(int *)current->data);
        } else if (current->type == TYPE_FLOAT) {
            printf("%f ", *(float *)current->data);
        } else if (current->type == TYPE_STRING) {
            printf("%s ", (char *)current->data);
        }
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    List list = {0};
    
    int *i1 = (int *)malloc(sizeof(int));
    if (!i1) return 1;
    *i1 = 42;
    list_append(&list, i1, TYPE_INT);
    
    float *f1 = (float *)malloc(sizeof(float));
    if (!f1) {
        list_free(&list);
        return 1;
    }
    *f1 = 3.14f;
    list_append(&list, f1, TYPE_FLOAT);
    
    char *s1 = (char *)malloc(16 * sizeof(char));
    if (!s1) {
        list_free(&list);
        return 1;
    }
    strcpy(s1, "2.71828");
    list_append(&list, s1, TYPE_STRING);
    
    char *s2 = (char *)malloc(16 * sizeof(char));
    if (!s2) {
        list_free(&list);
        return 1;
    }
    strcpy(s2, "hello");
    list_append(&list, s2, TYPE_STRING);
    
    int *i2 = (int *)malloc(sizeof(int));
    if (!i2) {
        list_free(&list);
        return 1;
    }
    *i2 = 100;
    list_append(&list, i2, TYPE_INT);
    
    printf("Before conversion:\n");
    print_list(&list);
    
    convert_all_to_float(&list);
    
    printf("After conversion:\n");
    print_list(&list);
    
    list_free(&list);
    return 0;
}