#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int count;
} RLE;

int encode_rle(const int *input, int input_len, RLE **output) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    if (input_len == 0) {
        *output = NULL;
        return 0;
    }
    
    int capacity = input_len;
    RLE *result = malloc(capacity * sizeof(RLE));
    if (result == NULL) {
        return -1;
    }
    
    int out_idx = 0;
    int current_val = input[0];
    int current_count = 1;
    
    for (int i = 1; i < input_len; i++) {
        if (input[i] == current_val) {
            current_count++;
        } else {
            if (out_idx >= capacity) {
                capacity *= 2;
                RLE *temp = realloc(result, capacity * sizeof(RLE));
                if (temp == NULL) {
                    free(result);
                    return -1;
                }
                result = temp;
            }
            result[out_idx].value = current_val;
            result[out_idx].count = current_count;
            out_idx++;
            current_val = input[i];
            current_count = 1;
        }
    }
    
    if (out_idx >= capacity) {
        capacity += 1;
        RLE *temp = realloc(result, capacity * sizeof(RLE));
        if (temp == NULL) {
            free(result);
            return -1;
        }
        result = temp;
    }
    result[out_idx].value = current_val;
    result[out_idx].count = current_count;
    out_idx++;
    
    RLE *final_output = realloc(result, out_idx * sizeof(RLE));
    if (final_output != NULL) {
        result = final_output;
    }
    
    *output = result;
    return out_idx;
}

int main(void) {
    int data[] = {1, 1, 1, 2, 2, 3, 3, 3, 3, 4};
    int data_len = sizeof(data) / sizeof(data[0]);
    RLE *encoded = NULL;
    
    int encoded_len = encode_rle(data, data_len, &encoded);
    
    if (encoded_len < 0) {
        fprintf(stderr, "Encoding failed\n");
        return 1;
    }
    
    for (int i = 0; i < encoded_len; i++) {
        printf("Value: %d, Count: %d\n", encoded[i].value, encoded[i].count);
    }
    
    free(encoded);
    encoded = NULL;
    
    return 0;
}