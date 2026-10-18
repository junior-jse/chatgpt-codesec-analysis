#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

typedef struct {
    void *data;
    size_t elem_size;
    size_t length;
    size_t capacity;
} RLEList;

typedef struct {
    void *value;
    size_t count;
} RLEPair;

RLEList* rle_create(size_t elem_size, size_t initial_capacity) {
    if (elem_size == 0 || initial_capacity == 0) {
        return NULL;
    }
    
    if (elem_size < sizeof(RLEPair)) {
        return NULL;
    }
    
    if (initial_capacity > SIZE_MAX / elem_size) {
        return NULL;
    }
    
    RLEList *list = malloc(sizeof(RLEList));
    if (list == NULL) {
        return NULL;
    }
    
    list->data = calloc(initial_capacity, elem_size);
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    
    list->elem_size = elem_size;
    list->length = 0;
    list->capacity = initial_capacity;
    
    return list;
}

void rle_destroy(RLEList *list) {
    if (list != NULL) {
        if (list->data != NULL) {
            for (size_t i = 0; i < list->length; i++) {
                RLEPair *pair = (RLEPair*)((char*)list->data + i * list->elem_size);
                free(pair->value);
            }
            free(list->data);
        }
        free(list);
    }
}

int rle_append(RLEList *list, const void *value, size_t count) {
    if (list == NULL || value == NULL || count == 0) {
        return -1;
    }
    
    if (list->elem_size < sizeof(RLEPair)) {
        return -1;
    }
    
    size_t value_size = list->elem_size - sizeof(size_t);
    
    if (list->length > 0) {
        RLEPair *last = (RLEPair*)((char*)list->data + (list->length - 1) * list->elem_size);
        if (last->value != NULL && memcmp(last->value, value, value_size) == 0) {
            if (SIZE_MAX - last->count < count) {
                return -1;
            }
            last->count += count;
            return 0;
        }
    }
    
    if (list->length >= list->capacity) {
        if (list->capacity > SIZE_MAX / 2) {
            return -1;
        }
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > SIZE_MAX / list->elem_size) {
            return -1;
        }
        void *new_data = realloc(list->data, new_capacity * list->elem_size);
        if (new_data == NULL) {
            return -1;
        }
        memset((char*)new_data + list->capacity * list->elem_size, 0, 
               (new_capacity - list->capacity) * list->elem_size);
        list->data = new_data;
        list->capacity = new_capacity;
    }
    
    RLEPair *pair = (RLEPair*)((char*)list->data + list->length * list->elem_size);
    pair->value = malloc(value_size);
    if (pair->value == NULL) {
        return -1;
    }
    
    if (value_size > 0) {
        if (value_size > list->elem_size - sizeof(size_t)) {
            free(pair->value);
            pair->value = NULL;
            return -1;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(pair->value, value, value_size);
    }
    pair->count = count;
    list->length++;
    
    return 0;
}

void rle_free_values(RLEList *list) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < list->length; i++) {
        RLEPair *pair = (RLEPair*)((char*)list->data + i * list->elem_size);
        free(pair->value);
        pair->value = NULL;
    }
}

int* decode_rle(const int *encoded, size_t encoded_len, size_t *decoded_len) {
    if (encoded == NULL || decoded_len == NULL || encoded_len == 0) {
        return NULL;
    }
    
    if (encoded_len % 2 != 0) {
        return NULL;
    }
    
    size_t total = 0;
    for (size_t i = 0; i < encoded_len; i += 2) {
        if (encoded[i] <= 0) {
            return NULL;
        }
        if (total > SIZE_MAX - (size_t)encoded[i]) {
            return NULL;
        }
        total += (size_t)encoded[i];
    }
    
    if (total == 0 || total > SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    
    int *result = malloc(total * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    size_t idx = 0;
    for (size_t i = 0; i < encoded_len; i += 2) {
        int count = encoded[i];
        int value = encoded[i + 1];
        
        for (int j = 0; j < count; j++) {
            if (idx >= total) {
                free(result);
                return NULL;
            }
            result[idx++] = value;
        }
    }
    
    if (idx != total) {
        free(result);
        return NULL;
    }
    
    *decoded_len = total;
    return result;
}

int* encode_rle(const int *data, size_t data_len, size_t *encoded_len) {
    if (data == NULL || encoded_len == NULL || data_len == 0) {
        return NULL;
    }
    
    if (data_len > SIZE_MAX / (2 * sizeof(int))) {
        return NULL;
    }
    
    size_t max_elements = data_len * 2;
    
    int *result = malloc(max_elements * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < data_len) {
        int current = data[i];
        size_t count = 1;
        
        while (i + count < data_len && data[i + count] == current) {
            count++;
        }
        
        if (count > (size_t)INT_MAX) {
            free(result);
            return NULL;
        }
        
        if (out_idx + 2 > max_elements) {
            free(result);
            return NULL;
        }
        
        result[out_idx++] = (int)count;
        result[out_idx++] = current;
        
        i += count;
    }
    
    *encoded_len = out_idx;
    return result;
}

int main(void) {
    int data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t data_len = sizeof(data) / sizeof(data[0]);
    size_t encoded_len = 0;
    size_t decoded_len = 0;
    
    int *encoded = encode_rle(data, data_len, &encoded_len);
    if (encoded == NULL) {
        fprintf(stderr, "Encoding failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: ");
    for (size_t i = 0; i < data_len; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    printf("Encoded: ");
    for (size_t i = 0; i < encoded_len; i++) {
        printf("%d ", encoded[i]);
    }
    printf("\n");
    
    int *decoded = decode_rle(encoded, encoded_len, &decoded_len);
    if (decoded == NULL) {
        fprintf(stderr, "Decoding failed\n");
        free(encoded);
        return EXIT_FAILURE;
    }
    
    printf("Decoded: ");
    for (size_t i = 0; i < decoded_len; i++) {
        printf("%d ", decoded[i]);
    }
    printf("\n");
    
    if (decoded_len == data_len && memcmp(data, decoded, data_len * sizeof(int)) == 0) {
        printf("Verification: PASSED\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(encoded);
    free(decoded);
    
    return EXIT_SUCCESS;
}