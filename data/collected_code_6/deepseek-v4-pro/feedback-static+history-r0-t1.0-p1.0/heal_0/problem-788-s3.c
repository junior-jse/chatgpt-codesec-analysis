#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int *list;
    size_t list_len;
} Tuple;

Tuple create_tuple(const char *str, const int *list, size_t list_len) {
    Tuple t;
    if (str == NULL || list == NULL || list_len == 0) {
        t.str = NULL;
        t.list = NULL;
        t.list_len = 0;
        return t;
    }
    
    t.str = malloc(strlen(str) + 1);
    if (t.str == NULL) {
        t.list = NULL;
        t.list_len = 0;
        return t;
    }
    strcpy(t.str, str);
    
    t.list = malloc(list_len * sizeof(int));
    if (t.list == NULL) {
        free(t.str);
        t.str = NULL;
        t.list_len = 0;
        return t;
    }
    memcpy(t.list, list, list_len * sizeof(int));
    t.list_len = list_len;
    
    return t;
}

void free_tuple(Tuple *t) {
    if (t == NULL) return;
    free(t->str);
    free(t->list);
    t->str = NULL;
    t->list = NULL;
    t->list_len = 0;
}

int main(void) {
    const char *str = "hello";
    int arr[] = {1, 2, 3, 4, 5};
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);
    
    Tuple t = create_tuple(str, arr, arr_len);
    if (t.str == NULL || t.list == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return EXIT_FAILURE;
    }
    
    printf("String: %s\n", t.str);
    printf("List: ");
    for (size_t i = 0; i < t.list_len; i++) {
        printf("%d ", t.list[i]);
    }
    printf("\n");
    
    free_tuple(&t);
    
    return EXIT_SUCCESS;
}