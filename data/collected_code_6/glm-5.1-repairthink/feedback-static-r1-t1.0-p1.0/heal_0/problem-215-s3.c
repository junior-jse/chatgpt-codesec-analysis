#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *decode_rle(const int *encoded, int encoded_len, int *decoded_len) {
    if (encoded == NULL || decoded_len == NULL) {
        return NULL;
    }

    size_t total_elements = 0;
    for (int i = 0; i < encoded_len; i += 2) {
        total_elements += (size_t)encoded[i];
    }

    int *decoded = (int *)malloc(total_elements * sizeof(int));
    if (decoded == NULL) {
        *decoded_len = 0;
        return NULL;
    }

    size_t index = 0;
    for (int i = 0; i < encoded_len; i += 2) {
        int count = encoded[i];
        int value = encoded[i + 1];
        for (int j = 0; j < count; j++) {
            decoded[index++] = value;
        }
    }

    *decoded_len = (int)total_elements;
    return decoded;
}

int main() {
    int encoded[] = {3, 1, 2, 4, 1, 2};
    int encoded_len = sizeof(encoded) / sizeof(encoded[0]);
    int decoded_len = 0;

    int *decoded = decode_rle(encoded, encoded_len, &decoded_len);

    if (decoded != NULL) {
        for (int i = 0; i < decoded_len; i++) {
            printf("%d ", decoded[i]);
        }
        printf("\n");
        free(decoded);
    } else {
        fprintf(stderr, "Decoding failed\n");
        return 1;
    }

    return 0;
}