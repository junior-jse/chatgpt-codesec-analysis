#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node **nested;
    int nested_count;
} Node;

int *extract_column(Node *list, int column_index, int *result_count) {
    if (!list || column_index < 0 || !result_count) {
        *result_count = 0;
        return NULL;
    }

    int capacity = 8;
    int count = 0;
    int *result = (int *)malloc(capacity * sizeof(int));
    if (!result) {
        *result_count = 0;
        return NULL;
    }

    for (int i = 0; i < list->nested_count; i++) {
        Node *current = list->nested[i];
        if (!current || column_index >= current->nested_count) {
            continue;
        }
        Node *target = current->nested[column_index];
        if (!target) {
            continue;
        }

        if (count == capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(result, capacity * sizeof(int));
            if (!temp) {
                free(result);
                *result_count = 0;
                return NULL;
            }
            result = temp;
        }
        result[count++] = target->value;
    }

    if (count == 0) {
        free(result);
        *result_count = 0;
        return NULL;
    }

    *result_count = count;
    return result;
}

Node *create_node(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->value = value;
    node->nested = NULL;
    node->nested_count = 0;
    return node;
}

Node *create_list(Node **items, int count) {
    Node *list = (Node *)malloc(sizeof(Node));
    if (!list) {
        return NULL;
    }
    list->value = -1;
    list->nested = NULL;
    list->nested_count = 0;

    if (count > 0) {
        list->nested = (Node **)malloc(count * sizeof(Node *));
        if (!list->nested) {
            free(list);
            return NULL;
        }
        list->nested_count = count;
        for (int i = 0; i < count; i++) {
            list->nested[i] = items[i];
        }
    }
    return list;
}

void free_node(Node *node) {
    if (!node) {
        return;
    }
    if (node->nested) {
        for (int i = 0; i < node->nested_count; i++) {
            free_node(node->nested[i]);
        }
        free(node->nested);
    }
    free(node);
}

int main(void) {
    Node *row1_col1 = create_node(1);
    Node *row1_col2 = create_node(2);
    Node *row1_items[] = {row1_col1, row1_col2};
    Node *row1 = create_list(row1_items, 2);

    Node *row2_col1 = create_node(3);
    Node *row2_col2 = create_node(4);
    Node *row2_items[] = {row2_col1, row2_col2};
    Node *row2 = create_list(row2_items, 2);

    Node *row3_col1 = create_node(5);
    Node *row3_col2 = create_node(6);
    Node *row3_items[] = {row3_col1, row3_col2};
    Node *row3 = create_list(row3_items, 2);

    Node *table_items[] = {row1, row2, row3};
    Node *table = create_list(table_items, 3);

    int result_count = 0;
    int *column = extract_column(table, 1, &result_count);

    printf("Extracted column values:\n");
    for (int i = 0; i < result_count; i++) {
        printf("%d\n", column[i]);
    }

    free(column);
    free_node(table);

    return 0;
}