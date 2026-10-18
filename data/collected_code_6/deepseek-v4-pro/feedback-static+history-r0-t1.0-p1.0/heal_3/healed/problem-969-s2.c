#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tuple {
    int initial;
    char *data;
    struct Tuple *next;
} Tuple;

Tuple *join_tuples(Tuple *head) {
    if (head == NULL) {
        return NULL;
    }
    Tuple *result = NULL;
    Tuple *result_tail = NULL;
    Tuple *current = head;
    while (current != NULL) {
        Tuple *existing = result;
        while (existing != NULL) {
            if (existing->initial == current->initial) {
                size_t existing_len = existing->data ? strlen(existing->data) : 0;
                size_t current_len = current->data ? strlen(current->data) : 0;
                if (existing_len > SIZE_MAX - current_len - 1) {
                    return NULL;
                }
                size_t total_len = existing_len + current_len;
                char *new_data = calloc(total_len + 1, sizeof(char));
                if (new_data == NULL) {
                    return NULL;
                }
                if (existing_len > 0) {
                    memcpy(new_data, existing->data, existing_len);
                }
                if (current_len > 0) {
                    memcpy(new_data + existing_len, current->data, current_len);
                }
                new_data[total_len] = '\0';
                free(existing->data);
                existing->data = new_data;
                break;
            }
            existing = existing->next;
        }
        if (existing == NULL) {
            Tuple *new_tuple = malloc(sizeof(Tuple));
            if (new_tuple == NULL) {
                return NULL;
            }
            new_tuple->initial = current->initial;
            new_tuple->data = NULL;
            new_tuple->next = NULL;
            if (current->data != NULL) {
                size_t data_len = strlen(current->data);
                if (data_len > SIZE_MAX - 1) {
                    free(new_tuple);
                    return NULL;
                }
                new_tuple->data = malloc(data_len + 1);
                if (new_tuple->data == NULL) {
                    free(new_tuple);
                    return NULL;
                }
                memcpy(new_tuple->data, current->data, data_len);
                new_tuple->data[data_len] = '\0';
            } else {
                new_tuple->data = malloc(1);
                if (new_tuple->data == NULL) {
                    free(new_tuple);
                    return NULL;
                }
                new_tuple->data[0] = '\0';
            }
            if (result == NULL) {
                result = new_tuple;
                result_tail = new_tuple;
            } else {
                result_tail->next = new_tuple;
                result_tail = new_tuple;
            }
        }
        current = current->next;
    }
    return result;
}

void free_tuple_list(Tuple *head) {
    Tuple *current = head;
    while (current != NULL) {
        Tuple *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

int main(void) {
    Tuple *head = NULL;
    Tuple *tail = NULL;
    int initials[] = {1, 2, 1, 3, 2};
    const char *datas[] = {"a", "b", "c", "d", "e"};
    size_t count = sizeof(initials) / sizeof(initials[0]);
    for (size_t i = 0; i < count; i++) {
        Tuple *new_tuple = malloc(sizeof(Tuple));
        if (new_tuple == NULL) {
            free_tuple_list(head);
            return 1;
        }
        new_tuple->initial = initials[i];
        new_tuple->data = NULL;
        new_tuple->next = NULL;
        if (datas[i] != NULL) {
            size_t data_len = strlen(datas[i]);
            if (data_len > SIZE_MAX - 1) {
                free(new_tuple);
                free_tuple_list(head);
                return 1;
            }
            new_tuple->data = malloc(data_len + 1);
            if (new_tuple->data == NULL) {
                free(new_tuple);
                free_tuple_list(head);
                return 1;
            }
            memcpy(new_tuple->data, datas[i], data_len);
            new_tuple->data[data_len] = '\0';
        } else {
            new_tuple->data = malloc(1);
            if (new_tuple->data == NULL) {
                free(new_tuple);
                free_tuple_list(head);
                return 1;
            }
            new_tuple->data[0] = '\0';
        }
        if (head == NULL) {
            head = new_tuple;
            tail = new_tuple;
        } else {
            tail->next = new_tuple;
            tail = new_tuple;
        }
    }
    Tuple *joined = join_tuples(head);
    Tuple *current = joined;
    while (current != NULL) {
        printf("%d: %s\n", current->initial, 
               current->data ? current->data : "");
        current = current->next;
    }
    free_tuple_list(head);
    free_tuple_list(joined);
    return 0;
}