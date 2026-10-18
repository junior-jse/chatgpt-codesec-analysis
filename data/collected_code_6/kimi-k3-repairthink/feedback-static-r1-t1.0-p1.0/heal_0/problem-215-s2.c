#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int value;
    int count;
} RLEPair;

int* decode_rle(const RLEPair* encoded, size_t encoded_size, size_t* decoded_size) {
    if (encoded == NULL || decoded_size == NULL) {
        return NULL;
    }
    
    if (encoded_size == 0) {
        *decoded_size = 0;
        return NULL;
    }
    
    size_t total_count = 0;
    for (size_t i = 0; i < encoded_size; i++) {
        if (encoded[i].count < 0) {
            return NULL;
        }
        if (encoded[i].count > 0 && total_count > SIZE_MAX - (size_t)encoded[i].count) {
            return NULL;
        }
        total_count += (size_t)encoded[i].count;
    }
    
    if (total_count == 0) {
        *decoded_size = 0;
        return NULL;
    }
    
    if (total_count > SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    
    int* decoded = (int*)malloc(total_count * sizeof(int));
    if (decoded == NULL) {
        return NULL;
    }
    
    size_t index = 0;
    for (size_t i = 0; i < encoded_size; i++) {
        for (int j = 0; j < encoded[i].count; j++) {
            decoded[index++] = encoded[i].value;
        }
    }
    
    *decoded_size = total_count;
    return decoded;
}

int main(void) {
    RLEPair encoded[] = {
        {1, 3},
        {2, 2},
        {3, 4},
        {4, 1}
    };
    size_t encoded_size = sizeof(encoded) / sizeof(encoded[0]);
    size_t decoded_size = 0;
    
    int* decoded = decode_rle(encoded, encoded_size, &decoded_size);
    
    if (decoded == NULL && decoded_size > 0) {
        fprintf(stderr, "Decoding failed\n");
        return EXIT_FAILURE;
    }
    
    if (decoded != NULL) {
        printf("Decoded list: ");
        for (size_t i = 0; i < decoded_size; i++) {
            printf("%d ", decoded[i]);
        }
        printf("\n");
        
        free(decoded);
        decoded = NULL;
    } else {
        printf("Decoded list is empty\n");
    }
    
    return EXIT_SUCCESS;
}