#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int value;
    int count;
} RLEPair;

int* decodeRLE(const RLEPair* pairs, size_t numPairs, size_t* outSize) {
    if (pairs == NULL || outSize == NULL || numPairs == 0) {
        return NULL;
    }

    size_t totalSize = 0;
    for (size_t i = 0; i < numPairs; i++) {
        if (pairs[i].count <= 0) {
            return NULL;
        }
        totalSize += (size_t)pairs[i].count;
    }

    int* result = (int*)malloc(totalSize * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    size_t index = 0;
    for (size_t i = 0; i < numPairs; i++) {
        for (int j = 0; j < pairs[i].count; j++) {
            result[index++] = pairs[i].value;
        }
    }

    *outSize = totalSize;
    return result;
}

int main(void) {
    RLEPair encoded[] = {
        {5, 3},
        {2, 2},
        {8, 4},
        {1, 1}
    };
    size_t numPairs = sizeof(encoded) / sizeof(encoded[0]);
    size_t decodedSize = 0;

    int* decoded = decodeRLE(encoded, numPairs, &decodedSize);
    if (decoded == NULL) {
        fprintf(stderr, "Decoding failed\n");
        return EXIT_FAILURE;
    }

    printf("Decoded list: ");
    for (size_t i = 0; i < decodedSize; i++) {
        printf("%d ", decoded[i]);
    }
    printf("\n");

    free(decoded);
    return EXIT_SUCCESS;
}