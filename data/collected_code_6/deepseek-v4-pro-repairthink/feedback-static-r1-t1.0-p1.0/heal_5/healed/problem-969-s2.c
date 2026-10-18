#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Tuple {
    int initial;
    char *data;
    size_t data_len;
    struct Tuple *next;
} Tuple;

void free_tuple_list(Tuple *head);

static void safe_memcpy(void *dest, size_t dest_size, const void *src, size_t n) {
    if (n > dest_size) {
        abort();
    }
    memcpy(dest, src, n);
}

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

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
                if (existing->data_len > SIZE_MAX - current->data_len) {
                    free_tuple_list(result);
                    return NULL;
                }
                size_t new_len = existing->data_len + current->data_len;
                char *new_data = malloc(new_len + 1);
                if (new_data == NULL) {
                    free_tuple_list(result);
                    return NULL;
                }
                safe_memcpy(new_data, new_len + 1, existing->data, existing->data_len);
                size_t remaining = new_len + 1 - existing->data_len;
                safe_memcpy(new_data + existing->data_len, remaining, current->data, current->data_len);
                new_data[new_len] = '\0';
                free(existing->data);
                existing->data = new_data;
                existing->data_len = new_len;
                break;
            }
            existing = existing->next;
        }
        if (existing == NULL) {
            Tuple *new_tuple = malloc(sizeof(Tuple));
            if (new_tuple == NULL) {
                free_tuple_list(result);
                return NULL;
            }
            new_tuple->initial = current->initial;
            new_tuple->data_len = current->data_len;
            new_tuple->data = malloc(new_tuple->data_len + 1);
            if (new_tuple->data == NULL) {
                free(new_tuple);
                free_tuple_list(result);
                return NULL;
            }
            safe_memcpy(new_tuple->data, new_tuple->data_len + 1, current->data, new_tuple->data_len);
            new_tuple->data[new_tuple->data_len] = '\0';
            new_tuple->next = NULL;
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
        size_t len = bounded_strlen(datas[i], 4096);
        if (len == 4096) {
            free(new_tuple);
            free_tuple_list(head);
            return 1;
        }
        new_tuple->data_len = len;
        new_tuple->data = malloc(len + 1);
        if (new_tuple->data == NULL) {
            free(new_tuple);
            free_tuple_list(head);
            return 1;
        }
        safe_memcpy(new_tuple->data, len + 1, datas[i], len);
        new_tuple->data[len] = '\0';
        new_tuple->next = NULL;
        if (head == NULL) {
            head = new_tuple;
            tail = new_tuple;
        } else {
            tail->next = new_tuple;
            tail = new_tuple;
        }
    }
    Tuple *joined = join_tuples(head);
    if (joined != NULL) {
        Tuple *current = joined;
        while (current != NULL) {
            printf("%d: %s\n", current->initial, current->data);
            current = current->next;
        }
    }
    free_tuple_list(head);
    free_tuple_list(joined);
    return 0;
}