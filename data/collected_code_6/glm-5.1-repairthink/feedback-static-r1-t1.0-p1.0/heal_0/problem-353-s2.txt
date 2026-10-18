#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
} List;

typedef struct {
    List *rows;
    int num_rows;
} NestedList;

NestedList remove_column(NestedList nested, int col) {
    if (col < 0) {
        return nested;
    }
    
    NestedList result;
    result.rows = (List *)malloc(nested.num_rows * sizeof(List));
    if (!result.rows) {
        exit(EXIT_FAILURE);
    }
    result.num_rows = nested.num_rows;
    
    for (int i = 0; i < nested.num_rows; i++) {
        if (col >= nested.rows[i].size) {
            result.rows[i].data = (int *)malloc(nested.rows[i].size * sizeof(int));
            if (!result.rows[i].data) {
                for (int j = 0; j < i; j++) free(result.rows[j].data);
                free(result.rows);
                exit(EXIT_FAILURE);
            }
            for (int k = 0; k < nested.rows[i].size; k++) {
                result.rows[i].data[k] = nested.rows[i].data[k];
            }
            result.rows[i].size = nested.rows[i].size;
        } else {
            int new_size = nested.rows[i].size - 1;
            result.rows[i].data = (int *)malloc(new_size * sizeof(int));
            if (!result.rows[i].data) {
                for (int j = 0; j < i; j++) free(result.rows[j].data);
                free(result.rows);
                exit(EXIT_FAILURE);
            }
            int k = 0;
            for (int j = 0; j < nested.rows[i].size; j++) {
                if (j != col) {
                    result.rows[i].data[k++] = nested.rows[i].data[j];
                }
            }
            result.rows[i].size = new_size;
        }
        free(nested.rows[i].data);
    }
    free(nested.rows);
    
    return result;
}

void free_nested_list(NestedList nested) {
    for (int i = 0; i < nested.num_rows; i++) {
        free(nested.rows[i].data);
    }
    free(nested.rows);
}

int main() {
    List rows[3];
    
    int r0[] = {1, 2, 3};
    rows[0].data = r0;
    rows[0].size = 3;
    
    int r1[] = {4, 5, 6};
    rows[1].data = r1;
    rows[1].size = 3;

    int r2[] = {7, 8, 9};
    rows[2].data = r2;
    rows[2].size = 3;

    NestedList nested;
    nested.rows = rows;
    nested.num_rows = 3;

    int col_to_remove = 1;
    NestedList modified = remove_column(nested, col_to_remove);

    for (int i = 0; i < modified.num_rows; i++) {
        for (int j = 0; j < modified.rows[i].size; j++) {
            printf("%d ", modified.rows[i].data[j]);
        }
        printf("\n");
    }

    free_nested_list(modified);

    return 0;
}