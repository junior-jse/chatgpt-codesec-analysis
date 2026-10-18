#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node **rows;
    size_t row_count;
    size_t *col_counts;
} NestedList;

Node *create_node(int data) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

void free_nested_list(NestedList *list) {
    if (list == NULL) {
        return;
    }
    if (list->rows != NULL) {
        for (size_t i = 0; i < list->row_count; i++) {
            free_list(list->rows[i]);
        }
        free(list->rows);
    }
    free(list->col_counts);
    free(list);
}

NestedList *create_nested_list(size_t row_count) {
    NestedList *list = malloc(sizeof(NestedList));
    if (list == NULL) {
        return NULL;
    }
    list->rows = calloc(row_count, sizeof(Node *));
    if (list->rows == NULL) {
        free(list);
        return NULL;
    }
    list->col_counts = calloc(row_count, sizeof(size_t));
    if (list->col_counts == NULL) {
        free(list->rows);
        free(list);
        return NULL;
    }
    list->row_count = row_count;
    return list;
}

int append_to_row(NestedList *list, size_t row, int data) {
    if (list == NULL || row >= list->row_count) {
        return -1;
    }
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return -1;
    }
    if (list->rows[row] == NULL) {
        list->rows[row] = new_node;
    } else {
        Node *current = list->rows[row];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->col_counts[row]++;
    return 0;
}

int *extract_column(const NestedList *list, size_t col_index, size_t *result_count) {
    if (list == NULL || result_count == NULL) {
        return NULL;
    }
    *result_count = 0;
    size_t valid_rows = 0;
    for (size_t i = 0; i < list->row_count; i++) {
        if (col_index < list->col_counts[i]) {
            valid_rows++;
        }
    }
    if (valid_rows == 0) {
        return NULL;
    }
    int *result = malloc(valid_rows * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    size_t result_idx = 0;
    for (size_t i = 0; i < list->row_count; i++) {
        if (col_index < list->col_counts[i]) {
            Node *current = list->rows[i];
            size_t current_col = 0;
            while (current != NULL && current_col < col_index) {
                current = current->next;
                current_col++;
            }
            if (current != NULL) {
                result[result_idx++] = current->data;
            }
        }
    }
    *result_count = result_idx;
    return result;
}

int main(void) {
    NestedList *list = create_nested_list(3);
    if (list == NULL) {
        fprintf(stderr, "Failed to create nested list\n");
        return EXIT_FAILURE;
    }

    if (append_to_row(list, 0, 1) != 0 ||
        append_to_row(list, 0, 2) != 0 ||
        append_to_row(list, 0, 3) != 0 ||
        append_to_row(list, 1, 4) != 0 ||
        append_to_row(list, 1, 5) != 0 ||
        append_to_row(list, 2, 6) != 0 ||
        append_to_row(list, 2, 7) != 0 ||
        append_to_row(list, 2, 8) != 0 ||
        append_to_row(list, 2, 9) != 0) {
        fprintf(stderr, "Failed to append data\n");
        free_nested_list(list);
        return EXIT_FAILURE;
    }

    size_t col_index = 1;
    size_t result_count = 0;
    int *column = extract_column(list, col_index, &result_count);

    if (column != NULL) {
        printf("Column %zu: ", col_index);
        for (size_t i = 0; i < result_count; i++) {
            printf("%d ", column[i]);
        }
        printf("\n");
        free(column);
    } else {
        printf("Column %zu not found or empty\n", col_index);
    }

    free_nested_list(list);
    return EXIT_SUCCESS;
}