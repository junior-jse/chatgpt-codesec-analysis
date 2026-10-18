#include <stdio.h>
#include <stdlib.h>

int *decode_rle(int *encoded, int encoded_size, int *decoded_size) {
    if (encoded == NULL || encoded_size <= 0 || encoded_size % 2 != 0) {
        *decoded_size = 0;
        return NULL;
    }
    
    int total_size = 0;
    for (int i = 0; i < encoded_size; i += 2) {
        if (encoded[i] < 0) {
            *decoded_size = 0;
            return NULL;
        }
        total_size += encoded[i];
    }
    
    if (total_size <= 0) {
        *decoded_size = 0;
        return NULL;
    }
    
    int *decoded = (int *)malloc(total_size * sizeof(int));
    if (decoded == NULL) {
        *decoded_size = 0;
        return NULL;
    }
    
    int decoded_index = 0;
    for (int i = 0; i < encoded_size; i += 2) {
        int count = encoded[i];
        int value = encoded[i + 1];
        for (int j = 0; j < count; j++) {
            decoded[decoded_index++] = value;
        }
    }
    
    *decoded_size = total_size;
    return decoded;
}

int main() {
    int encoded[] = {3, 1, 2, 5, 4, 2};
    int encoded_size = sizeof(encoded) / sizeof(encoded[0]);
    int decoded_size = 0;
    
    int *decoded = decode_rle(encoded, encoded_size, &decoded_size);
    
    if (decoded != NULL) {
        printf("Decoded array (size %d): ", decoded_size);
        for (int i = 0; i < decoded_size; i++) {
            printf("%d ", decoded[i]);
        }
        printf("\n");
        free(decoded);
    } else {
        printf("Decoding failed\n");
    }
    
    return 0;
}