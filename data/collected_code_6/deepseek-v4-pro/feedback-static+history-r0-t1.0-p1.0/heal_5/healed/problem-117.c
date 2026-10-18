#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

typedef struct Node {
    void *data;
    int type;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t size;
} List;

List *list_create(void) {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

void list_destroy(List *list) {
    if (list == NULL) return;
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

int list_push(List *list, void *data, int type) {
    if (list == NULL || data == NULL) return -1;

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) return -1;

    new_node->data = data;
    new_node->type = type;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->size++;
    return 0;
}

int is_valid_float_string(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    char *endptr = NULL;
    errno = 0;
    strtod(str, &endptr);
    
    if (errno == ERANGE) return 0;
    if (endptr == str) return 0;
    
    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr)) return 0;
        endptr++;
    }
    
    return 1;
}

void convert_to_float(List *list) {
    if (list == NULL) return;
    
    Node *current = list->head;
    while (current != NULL) {
        if (current->type == 1) {
            current = current->next;
            continue;
        }
        
        if (current->type == 2) {
            int int_val = *(int *)current->data;
            float *new_data = (float *)malloc(sizeof(float));
            if (new_data == NULL) {
                current = current->next;
                continue;
            }
            *new_data = (float)int_val;
            free(current->data);
            current->data = new_data;
            current->type = 1;
        }
        else if (current->type == 3) {
            char *str_val = (char *)current->data;
            if (is_valid_float_string(str_val)) {
                char *endptr = NULL;
                errno = 0;
                double val = strtod(str_val, &endptr);
                if (errno != ERANGE) {
                    float *new_data = (float *)malloc(sizeof(float));
                    if (new_data != NULL) {
                        *new_data = (float)val;
                        free(current->data);
                        current->data = new_data;
                        current->type = 1;
                    }
                }
            }
        }
        else if (current->type == 4) {
            double double_val = *(double *)current->data;
            float *new_data = (float *)malloc(sizeof(float));
            if (new_data == NULL) {
                current = current->next;
                continue;
            }
            *new_data = (float)double_val;
            free(current->data);
            current->data = new_data;
            current->type = 1;
        }
        
        current = current->next;
    }
}

void print_list(List *list) {
    if (list == NULL) return;
    
    Node *current = list->head;
    while (current != NULL) {
        if (current->type == 1) {
            printf("%f ", *(float *)current->data);
        } else if (current->type == 2) {
            printf("%d ", *(int *)current->data);
        } else if (current->type == 3) {
            printf("%s ", (char *)current->data);
        } else if (current->type == 4) {
            printf("%f ", *(double *)current->data);
        }
        current = current->next;
    }
    printf("\n");
}

char *safe_strdup(const char *src, size_t max_len) {
    if (src == NULL) return NULL;
    size_t len = strnlen(src, max_len);
    char *dest = (char *)malloc(len + 1);
    if (dest == NULL) return NULL;
    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

int main(void) {
    List *list = list_create();
    if (list == NULL) return 1;
    
    int *int_val = (int *)malloc(sizeof(int));
    if (int_val != NULL) {
        *int_val = 42;
        if (list_push(list, int_val, 2) != 0) {
            free(int_val);
        }
    }
    
    char *str_val1 = safe_strdup("3.14159", 31);
    if (str_val1 != NULL) {
        if (list_push(list, str_val1, 3) != 0) {
            free(str_val1);
        }
    }
    
    double *double_val = (double *)malloc(sizeof(double));
    if (double_val != NULL) {
        *double_val = 2.718281828;
        if (list_push(list, double_val, 4) != 0) {
            free(double_val);
        }
    }
    
    float *float_val = (float *)malloc(sizeof(float));
    if (float_val != NULL) {
        *float_val = 1.5f;
        if (list_push(list, float_val, 1) != 0) {
            free(float_val);
        }
    }
    
    char *str_val2 = safe_strdup("not_a_number", 31);
    if (str_val2 != NULL) {
        if (list_push(list, str_val2, 3) != 0) {
            free(str_val2);
        }
    }
    
    char *str_val3 = safe_strdup("123.456", 31);
    if (str_val3 != NULL) {
        if (list_push(list, str_val3, 3) != 0) {
            free(str_val3);
        }
    }
    
    printf("Before conversion: ");
    print_list(list);
    
    convert_to_float(list);
    
    printf("After conversion: ");
    print_list(list);
    
    list_destroy(list);
    
    return 0;
}