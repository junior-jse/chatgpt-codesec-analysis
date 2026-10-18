#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int count;
    int value;
} RLEPair;

int* rle_decode(const RLEPair* encoded, int encoded_len, int* decoded_len) {
    if (encoded == NULL || decoded_len == NULL) {
        return NULL;
    }

    size_t total_count = 0;
    for (int i = 0; i < encoded_len; ++i) {
        if (encoded[i].count < 0) {
            return NULL;
        }
        total_count += (size_t)encoded[i].count;
    }

    if (total_count == 0) {
        *decoded_len = 0;
        int* result = (int*)malloc(sizeof(int));
        if (result == NULL) {
            return NULL;
        }
        return result;
    }

    int* decoded = (int*)malloc(total_count * sizeof(int));
    if (decoded == NULL) {
        return NULL;
    }

    int current_index = 0;
    for (int i = 0; i < encoded_len; ++i) {
        for (int j = 0; j < encoded[i].count; ++j) {
            decoded[current_index++] = encoded[i].value;
        }
    }

    *decoded_len = (int)total_count;
    return decoded;
}

int main() {
    RLEPair encoded[] = {
        {3, 1},
        {2, 4},
        {1, 5},
        {4, 2}
    };

    int decoded_len = 0;
    int* decoded = rle_decode(encoded, 4, &decoded_len);

    if (decoded != NULL) {
        for (int i = 0; i < decoded_len; ++i) {
            printf("%d ", decoded[i]);
        }
        printf("\n");
        free(decoded);
    }

    return 0;
}