#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int key;
    int value;
} Pair;

int convert_tuple_to_dict(int tuple[], int size, Pair dict[]) {
    if (tuple == NULL || dict == NULL || size < 0 || size % 2 != 0 || size > MAX_SIZE) {
        return -1;
    }
    
    int dict_size = size / 2;
    for (int i = 0; i < dict_size; i++) {
        dict[i].key = tuple[i * 2];
        dict[i].value = tuple[i * 2 + 1];
    }
    
    return dict_size;
}

int main(void) {
    int tuple[] = {1, 10, 2, 20, 3, 30, 4, 40, 5, 50};
    int size = sizeof(tuple) / sizeof(tuple[0]);
    Pair dict[MAX_SIZE / 2];
    
    int dict_size = convert_tuple_to_dict(tuple, size, dict);
    
    if (dict_size < 0) {
        fprintf(stderr, "Error: Invalid input parameters\n");
        return EXIT_FAILURE;
    }
    
    printf("Dictionary (key-value pairs):\n");
    for (int i = 0; i < dict_size; i++) {
        printf("Key: %d, Value: %d\n", dict[i].key, dict[i].value);
    }
    
    return EXIT_SUCCESS;
}