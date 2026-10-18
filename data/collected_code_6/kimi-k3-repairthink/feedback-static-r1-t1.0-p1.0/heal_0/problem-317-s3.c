#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int value;
    int count;
} RunLengthPair;

typedef struct {
    RunLengthPair* pairs;
    size_t size;
    size_t capacity;
} RunLengthList;

RunLengthList* run_length_list_create(size_t initial_capacity) {
    RunLengthList* list = (RunLengthList*)malloc(sizeof(RunLengthList));
    if (list == NULL) {
        return NULL;
    }
    
    list->pairs = (RunLengthPair*)malloc(initial_capacity * sizeof(RunLengthPair));
    if (list->pairs == NULL) {
        free(list);
        return NULL;
    }
    
    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

int run_length_list_append(RunLengthList* list, int value, int count) {
    if (list == NULL || count <= 0) {
        return -1;
    }
    
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        RunLengthPair* new_pairs = (RunLengthPair*)realloc(list->pairs, new_capacity * sizeof(RunLengthPair));
        if (new_pairs == NULL) {
            return -1;
        }
        list->pairs = new_pairs;
        list->capacity = new_capacity;
    }
    
    list->pairs[list->size].value = value;
    list->pairs[list->size].count = count;
    list->size++;
    return 0;
}

void run_length_list_free(RunLengthList* list) {
    if (list != NULL) {
        free(list->pairs);
        free(list);
    }
}

RunLengthList* modified_run_length_encode(const int* data, size_t length) {
    if (data == NULL || length == 0) {
        return NULL;
    }
    
    RunLengthList* result = run_length_list_create(16);
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    while (i < length) {
        int current_value = data[i];
        int count = 1;
        
        while (i + count < length && data[i + count] == current_value && count < 255) {
            count++;
        }
        
        if (count > 1) {
            if (run_length_list_append(result, current_value, count) != 0) {
                run_length_list_free(result);
                return NULL;
            }
        } else {
            int literal_count = 0;
            size_t literal_start = i;
            
            while (i < length && literal_count < 128) {
                if (i + 1 < length && data[i] == data[i + 1]) {
                    break;
                }
                literal_count++;
                i++;
            }
            
            for (int j = 0; j < literal_count; j++) {
                if (run_length_list_append(result, data[literal_start + j], 1) != 0) {
                    run_length_list_free(result);
                    return NULL;
                }
            }
            continue;
        }
        
        i += count;
    }
    
    return result;
}

int* modified_run_length_decode(const RunLengthList* encoded, size_t* output_length) {
    if (encoded == NULL || output_length == NULL) {
        return NULL;
    }
    
    size_t total_length = 0;
    for (size_t i = 0; i < encoded->size; i++) {
        total_length += encoded->pairs[i].count;
    }
    
    if (total_length == 0) {
        *output_length = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(total_length * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    size_t index = 0;
    for (size_t i = 0; i < encoded->size; i++) {
        for (int j = 0; j < encoded->pairs[i].count; j++) {
            result[index++] = encoded->pairs[i].value;
        }
    }
    
    *output_length = total_length;
    return result;
}

void print_array(const int* data, size_t length) {
    if (data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < length; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

void print_encoded(const RunLengthList* encoded) {
    if (encoded == NULL) {
        return;
    }
    
    printf("Encoded: ");
    for (size_t i = 0; i < encoded->size; i++) {
        printf("(%d,%d) ", encoded->pairs[i].value, encoded->pairs[i].count);
    }
    printf("\n");
}

int main(void) {
    int test_data[] = {1, 1, 1, 2, 3, 4, 4, 4, 4, 5, 6, 6};
    size_t data_length = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("Original: ");
    print_array(test_data, data_length);
    
    RunLengthList* encoded = modified_run_length_encode(test_data, data_length);
    if (encoded == NULL) {
        fprintf(stderr, "Encoding failed\n");
        return EXIT_FAILURE;
    }
    
    print_encoded(encoded);
    
    size_t decoded_length = 0;
    int* decoded = modified_run_length_decode(encoded, &decoded_length);
    if (decoded == NULL && decoded_length > 0) {
        fprintf(stderr, "Decoding failed\n");
        run_length_list_free(encoded);
        return EXIT_FAILURE;
    }
    
    printf("Decoded:  ");
    print_array(decoded, decoded_length);
    
    if (decoded_length == data_length && memcmp(test_data, decoded, data_length * sizeof(int)) == 0) {
        printf("Verification: PASSED\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(decoded);
    run_length_list_free(encoded);
    
    return EXIT_SUCCESS;
}