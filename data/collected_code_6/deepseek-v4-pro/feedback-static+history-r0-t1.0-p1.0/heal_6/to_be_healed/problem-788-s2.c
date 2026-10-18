#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int *list;
    int list_size;
} Tuple;

Tuple create_tuple(const char *str, const int *list, int list_size) {
    Tuple t;
    size_t str_len;
    size_t copy_len;
    
    if (str == NULL) {
        str_len = 0;
    } else {
        str_len = strnlen(str, 4096);
    }
    
    copy_len = str_len;
    if (copy_len > 4096) {
        copy_len = 4096;
    }
    
    t.str = malloc(copy_len + 1);
    if (t.str == NULL) {
        fprintf(stderr, "Memory allocation failed for string\n");
        exit(EXIT_FAILURE);
    }
    
    if (str != NULL && copy_len > 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(t.str, str, copy_len);
    }
    t.str[copy_len] = '\0';
    
    t.list_size = list_size;
    if (list_size > 0 && list != NULL) {
        if (list_size > 4096) {
            free(t.str);
            fprintf(stderr, "List size exceeds maximum allowed\n");
            exit(EXIT_FAILURE);
        }
        t.list = malloc(sizeof(int) * list_size);
        if (t.list == NULL) {
            free(t.str);
            fprintf(stderr, "Memory allocation failed for list\n");
            exit(EXIT_FAILURE);
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(t.list, list, sizeof(int) * list_size);
    } else {
        t.list = NULL;
        t.list_size = 0;
    }
    
    return t;
}

void free_tuple(Tuple *t) {
    if (t != NULL) {
        free(t->str);
        free(t->list);
        t->str = NULL;
        t->list = NULL;
        t->list_size = 0;
    }
}

int main(void) {
    const char *str = "hello";
    int list_data[] = {1, 2, 3, 4, 5};
    int list_size = sizeof(list_data) / sizeof(list_data[0]);
    
    Tuple t = create_tuple(str, list_data, list_size);
    
    printf("String: %s\n", t.str);
    printf("List: ");
    for (int i = 0; i < t.list_size; i++) {
        printf("%d ", t.list[i]);
    }
    printf("\n");
    
    free_tuple(&t);
    
    return 0;
}