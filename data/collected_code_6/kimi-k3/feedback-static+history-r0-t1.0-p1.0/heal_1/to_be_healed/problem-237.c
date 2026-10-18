#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TUPLES 100
#define MAX_TUPLE_SIZE 10

typedef struct {
    int data[MAX_TUPLE_SIZE];
    int size;
    /* Possible weaknesses found:
     *  struct member 'Tuple::count' is never used. [unusedStructMember]
     */
    int count;
} Tuple;

int compare_tuples(const int *a, const int *b, int size) {
    if (a == NULL || b == NULL || size <= 0) {
        return 0;
    }
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int find_similar_tuples(Tuple tuples[], int n, int target_count) {
    if (tuples == NULL || n <= 0 || n > MAX_TUPLES || target_count < 0) {
        return -1;
    }
    
    int result = 0;
    int processed[MAX_TUPLES] = {0};
    
    for (int i = 0; i < n; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        processed[i] = 1;
        
        for (int j = i + 1; j < n; j++) {
            if (!processed[j] && tuples[i].size == tuples[j].size &&
                compare_tuples(tuples[i].data, tuples[j].data, tuples[i].size)) {
                count++;
                processed[j] = 1;
            }
        }
        
        if (count == target_count) {
            result++;
        }
    }
    
    return result;
}

int main(void) {
    Tuple tuples[MAX_TUPLES];
    int n;
    int target_count;
    
    printf("Enter number of tuples (max %d): ", MAX_TUPLES);
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_TUPLES) {
        fprintf(stderr, "Invalid number of tuples\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < n; i++) {
        printf("Enter size of tuple %d (max %d): ", i + 1, MAX_TUPLE_SIZE);
        if (scanf("%d", &tuples[i].size) != 1 || 
            tuples[i].size <= 0 || tuples[i].size > MAX_TUPLE_SIZE) {
            fprintf(stderr, "Invalid tuple size\n");
            return EXIT_FAILURE;
        }
        
        printf("Enter %d elements: ", tuples[i].size);
        for (int j = 0; j < tuples[i].size; j++) {
            if (scanf("%d", &tuples[i].data[j]) != 1) {
                fprintf(stderr, "Invalid element\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    printf("Enter target occurrence count: ");
    if (scanf("%d", &target_count) != 1 || target_count < 0) {
        fprintf(stderr, "Invalid target count\n");
        return EXIT_FAILURE;
    }
    
    int result = find_similar_tuples(tuples, n, target_count);
    
    if (result < 0) {
        fprintf(stderr, "Error processing tuples\n");
        return EXIT_FAILURE;
    }
    
    printf("Number of tuples occurring %d times: %d\n", target_count, result);
    
    return EXIT_SUCCESS;
}