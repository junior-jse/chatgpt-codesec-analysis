#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    int size;
} Tuple;

void remove_all_none_tuples(Tuple **list, int *count) {
    if (list == NULL || *list == NULL || count == NULL || *count <= 0) {
        return;
    }
    
    int write_idx = 0;
    for (int read_idx = 0; read_idx < *count; read_idx++) {
        int has_value = 0;
        for (int i = 0; i < (*list)[read_idx].size; i++) {
            if ((*list)[read_idx].values[i] != 0) {
                has_value = 1;
                break;
            }
        }
        
        if (has_value) {
            if (write_idx != read_idx) {
                (*list)[write_idx] = (*list)[read_idx];
            }
            write_idx++;
        } else {
            free((*list)[read_idx].values);
            (*list)[read_idx].values = NULL;
        }
    }
    
    if (write_idx < *count) {
        Tuple *temp = realloc(*list, write_idx * sizeof(Tuple));
        if (write_idx == 0) {
            free(*list);
            *list = NULL;
        } else if (temp != NULL) {
            *list = temp;
        }
    }
    
    *count = write_idx;
}

int main(void) {
    Tuple *list = malloc(4 * sizeof(Tuple));
    if (list == NULL) {
        return EXIT_FAILURE;
    }
    
    int count = 4;
    
    list[0].size = 3;
    list[0].values = malloc(3 * sizeof(int));
    if (list[0].values == NULL) {
        free(list);
        return EXIT_FAILURE;
    }
    list[0].values[0] = 0;
    list[0].values[1] = 0;
    list[0].values[2] = 0;
    
    list[1].size = 2;
    list[1].values = malloc(2 * sizeof(int));
    if (list[1].values == NULL) {
        free(list[0].values);
        free(list);
        return EXIT_FAILURE;
    }
    list[1].values[0] = 1;
    list[1].values[1] = 2;
    
    list[2].size = 3;
    list[2].values = malloc(3 * sizeof(int));
    if (list[2].values == NULL) {
        free(list[0].values);
        free(list[1].values);
        free(list);
        return EXIT_FAILURE;
    }
    list[2].values[0] = 0;
    list[2].values[1] = 0;
    list[2].values[2] = 0;
    
    list[3].size = 2;
    list[3].values = malloc(2 * sizeof(int));
    if (list[3].values == NULL) {
        free(list[0].values);
        free(list[1].values);
        free(list[2].values);
        free(list);
        return EXIT_FAILURE;
    }
    list[3].values[0] = 3;
    list[3].values[1] = 4;
    
    remove_all_none_tuples(&list, &count);
    
    printf("Remaining tuples: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Tuple %d: ", i);
        for (int j = 0; j < list[i].size; j++) {
            printf("%d ", list[i].values[j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < count; i++) {
        free(list[i].values);
    }
    free(list);
    
    return EXIT_SUCCESS;
}