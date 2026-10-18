#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* decode_rle(const int *encoded, int encoded_size, int *decoded_size) {
    if (encoded == NULL || encoded_size <= 0 || decoded_size == NULL) {
        return NULL;
    }
    
    int total_size = 0;
    for (int i = 0; i < encoded_size; i += 2) {
        total_size += encoded[i];
    }
    
    int *decoded = (int*)malloc(total_size * sizeof(int));
    if (decoded == NULL) {
        return NULL;
    }
    
    int index = 0;
    for (int i = 0; i < encoded_size; i += 2) {
        int count = encoded[i];
        int value = encoded[i + 1];
        for (int j = 0; j < count; j++) {
            decoded[index++] = value;
        }
    }
    
    *decoded_size = total_size;
    return decoded;
}

int main(void) {
    int encoded[] = {3, 5, 2, 7, 1, 9};
    int encoded_size = sizeof(encoded) / sizeof(encoded[0]);
    int decoded_size = 0;
    
    int *decoded = decode_rle(encoded, encoded_size, &decoded_size);
    if (decoded == NULL) {
        printf("Decoding failed\n");
        return 1;
    }
    
    printf("Decoded array: ");
    for (int i = 0; i < decoded_size; i++) {
        printf("%d ", decoded[i]);
    }
    printf("\n");
    
    free(decoded);
    return 0;
}