#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int *list;
    size_t list_size;
} Tuple;

Tuple* create_tuple(const char *str, const int *list, size_t list_size) {
    Tuple *new_tuple = (Tuple*)malloc(sizeof(Tuple));
    if (new_tuple == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    new_tuple->str = (char*)malloc(strlen(str) + 1);
    if (new_tuple->str == NULL) {
        free(new_tuple);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(new_tuple->str, str);

    new_tuple->list = (int*)malloc(list_size * sizeof(int));
    if (new_tuple->list == NULL) {
        free(new_tuple->str);
        free(new_tuple);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(new_tuple->list, list, list_size * sizeof(int));
    new_tuple->list_size = list_size;

    return new_tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        free(tuple->str);
        free(tuple->list);
        free(tuple);
    }
}

int main() {
    const char *my_string = "example";
    int my_list[] = {1, 2, 3, 4, 5};
    size_t list_size = sizeof(my_list) / sizeof(my_list[0]);

    Tuple *my_tuple = create_tuple(my_string, my_list, list_size);
    if (my_tuple == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return EXIT_FAILURE;
    }

    printf("String: %s\n", my_tuple->str);
    printf("List: ");
    for (size_t i = 0; i < my_tuple->list_size; i++) {
        printf("%d ", my_tuple->list[i]);
    }
    printf("\n");

    free_tuple(my_tuple);
    return EXIT_SUCCESS;
}