#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    len = strlen(input);
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
    const char *test_string = "AAABBBCCDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD