#include <stdio.h>
#include <stdlib.h>

int *decode_rle(const int *encoded, size_t encoded_len, size_t *result_len) {
    if (encoded == NULL || result_len == NULL) {
        return NULL;
    }

    size_t total_len = 0;
    for (size_t i = 0; i < encoded_len; i += 2) {
        if (encoded[i] < 0) {
            return NULL;
        }
        if (total_len > SIZE_MAX - (size_t)encoded[i]) {
            return NULL;
        }
        total_len += (size_t)encoded[i];
    }

    int *result = (int *)malloc(total_len * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    size_t res_idx = 0;
    for (size_t i = 0; i < encoded_len; i += 2) {
        for (int count = 0; count < encoded[i]; count++) {
            result[res_idx++] = encoded[i + 1];
        }
    }

    *result_len = total_len;
    return result;
}

int main() {
    int encoded[] = {3, 1, 4, 2, 1, 0};
    size_t encoded_len = sizeof(encoded) / sizeof(encoded[0]);
    size_t result_len = 0;

    int *decoded = decode_rle(encoded, encoded_len, &result_len);

    if (decoded != NULL) {
        for (size_t i = 0; i < result_len; i++) {
            printf("%d ", decoded[i]);
        }
        printf("\n");
        free(decoded);
    } else {
        printf("Decoding failed.\n");
    }

    return 0;
}