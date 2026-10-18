#include <stdio.h>
#include <stdlib.h>

char **convert_tuple_to_list(char **tuple, size_t tuple_size, const char *str, size_t *list_size) {
    if (tuple == NULL || str == NULL || list_size == NULL) {
        return NULL;
    }

    size_t new_size = tuple_size * 2;
    char **list = calloc(new_size, sizeof(char *));
    if (list == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < tuple_size; i++) {
        size_t idx = i * 2;
        
        if (tuple[i] != NULL) {
            int elem_len_int = snprintf(NULL, 0, "%s", tuple[i]);
            if (elem_len_int < 0) {
                for (size_t j = 0; j < new_size; j++) {
                    free(list[j]);
                }
                free(list);
                return NULL;
            }
            size_t elem_len = (size_t)elem_len_int;
            list[idx] = malloc(elem_len + 1);
            if (list[idx] == NULL) {
                for (size_t j = 0; j < new_size; j++) {
                    free(list[j]);
                }
                free(list);
                return NULL;
            }
            snprintf(list[idx], elem_len + 1, "%s", tuple[i]);
        }

        int str_len_int = snprintf(NULL, 0, "%s", str);
        if (str_len_int < 0) {
            for (size_t j = 0; j < new_size; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        size_t str_len = (size_t)str_len_int;
        list[idx + 1] = malloc(str_len + 1);
        if (list[idx + 1] == NULL) {
            for (size_t j = 0; j < new_size; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        snprintf(list[idx + 1], str_len + 1, "%s", str);
    }

    *list_size = new_size;
    return list;
}

int main(void) {
    char *tuple[] = {"apple", "banana", "cherry"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    const char *str = "INSERT";
    size_t list_size = 0;

    char **list = convert_tuple_to_list(tuple, tuple_size, str, &list_size);
    
    if (list != NULL) {
        for (size_t i = 0; i < list_size; i++) {
            if (list[i] != NULL) {
                printf("%s\n", list[i]);
            }
            free(list[i]);
        }
        free(list);
    }

    return 0;
}