#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    char value;
    int count;
} Run;

int rle_encode(const char *input, size_t input_max_len, Run *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0 || input_max_len == 0) {
        return -1;
    }

    size_t input_len = strnlen(input, input_max_len);
    if (input_len == 0) {
        return 0;
    }

    size_t output_count = 0;
    size_t i = 0;

    while (i < input_len) {
        if (output_count >= output_size) {
            return -1;
        }

        char current = input[i];
        int count = 1;

        while (i + (size_t)count < input_len && input[i + (size_t)count] == current && count < INT_MAX) {
            count++;
        }

        output[output_count].value = current;
        output[output_count].count = count;
        output_count++;

        i += (size_t)count;
    }

    return (int)output_count;
}

int rle_decode(const Run *input, size_t input_size, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }

    size_t output_index = 0;

    for (size_t i = 0; i < input_size; i++) {
        if (input[i].count <= 0) {
            return -1;
        }

        if ((size_t)input[i].count > output_size - 1 - output_index) {
            return -1;
        }

        for (int j = 0; j < input[i].count; j++) {
            output[output_index++] = input[i].value;
        }
    }

    output[output_index] = '\0';
    return (int)output_index;
}

int main(void) {
    const char test_string[] = "AAABBBCCDDDDDDDDDDDD";
    size_t input_len = strnlen(test_string, sizeof(test_string));

    if (input_len == 0 || input_len > SIZE_MAX / sizeof(Run)) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }

    Run *encoded = malloc(input_len * sizeof(Run));
    if (encoded == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    int encoded_count = rle_encode(test_string, sizeof(test_string), encoded, input_len);
    if (encoded_count < 0) {
        fprintf(stderr, "Encoding failed\n");
        free(encoded);
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", test_string);
    printf("Encoded: ");
    for (int i = 0; i < encoded_count; i++) {
        printf("%c%d", encoded[i].value, encoded[i].count);
    }
    printf("\n");

    size_t decode_buffer_size = input_len + 1;
    char *decoded = malloc(decode_buffer_size);
    if (decoded == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(encoded);
        return EXIT_FAILURE;
    }

    int decoded_len = rle_decode(encoded, (size_t)encoded_count, decoded, decode_buffer_size);
    if (decoded_len < 0) {
        fprintf(stderr, "Decoding failed\n");
        free(encoded);
        free(decoded);
        return EXIT_FAILURE;
    }

    printf("Decoded: %s\n", decoded);

    if ((size_t)decoded_len == input_len && memcmp(test_string, decoded, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }

    free(encoded);
    free(decoded);

    return EXIT_SUCCESS;
}