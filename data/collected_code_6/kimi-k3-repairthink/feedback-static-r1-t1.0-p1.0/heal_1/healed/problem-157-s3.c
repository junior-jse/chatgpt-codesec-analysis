#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 4096

typedef struct {
    char value;
    int count;
} RLEPair;

size_t rle_encode(const char *input, RLEPair **output) {
    size_t len;
    size_t i;
    size_t pair_count;
    RLEPair *pairs;

    if (input == NULL || output == NULL) {
        return 0;
    }

    len = 0;
    while (len < MAX_INPUT_LEN && input[len] != '\0') {
        len++;
    }

    if (len == 0) {
        *output = NULL;
        return 0;
    }

    pairs = (RLEPair *)malloc(len * sizeof(RLEPair));
    if (pairs == NULL) {
        *output = NULL;
        return 0;
    }

    pair_count = 0;
    pairs[0].value = input[0];
    pairs[0].count = 1;

    for (i = 1; i < len; i++) {
        if (input[i] == pairs[pair_count].value) {
            pairs[pair_count].count++;
        } else {
            pair_count++;
            pairs[pair_count].value = input[i];
            pairs[pair_count].count = 1;
        }
    }

    pair_count++;

    *output = pairs;
    return pair_count;
}

int main(void) {
    const char *test_string = "AAABBBCCDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
    RLEPair *encoded = NULL;
    size_t count;
    size_t i;

    count = rle_encode(test_string, &encoded);

    for (i = 0; i < count; i++) {
        printf("%c%d", encoded[i].value, encoded[i].count);
    }
    printf("\n");

    free(encoded);

    return 0;
}