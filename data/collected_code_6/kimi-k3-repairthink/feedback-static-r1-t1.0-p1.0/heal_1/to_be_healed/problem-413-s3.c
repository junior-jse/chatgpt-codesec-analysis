#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char name[50];
    double value;
} Tuple;

/* Possible weaknesses found:
 *  Parameter 'list' can be declared as pointer to const [constParameterPointer]
 */
int extract_nth_element(Tuple *list, size_t size, size_t n, Tuple *result) {
    if (list == NULL || result == NULL) {
        return -1;
    }
    if (n >= size) {
        return -1;
    }
    *result = list[n];
    return 0;
}

int main(void) {
    Tuple list[] = {
        {1, "Alice", 100.5},
        {2, "Bob", 200.75},
        {3, "Charlie", 300.25},
        {4, "Diana", 400.0},
        {5, "Eve", 500.5}
    };
    size_t size = sizeof(list) / sizeof(list[0]);
    size_t n = 2;
    Tuple result;
    
    if (extract_nth_element(list, size, n, &result) == 0) {
        printf("Element at index %zu: id=%d, name=%s, value=%.2f\n", 
               n, result.id, result.name, result.value);
    } else {
        fprintf(stderr, "Error: Invalid index or NULL pointer\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}