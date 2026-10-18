#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 100

static int is_duplicate(const char *words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int remove_duplicate_words(const char *input[], int input_count, const char *output[], int max_output) {
    int output_count = 0;
    for (int i = 0; i < input_count; i++) {
        if (input[i] == NULL) {
            continue;
        }
        if (output_count >= max_output) {
            break;
        }
        if (!is_duplicate(output, output_count, input[i])) {
            output[output_count] = input[i];
            output_count++;
        }
    }
    return output_count;
}

int main(void) {
    const char *input[] = {"apple", "banana", "apple", "cherry", "banana", "date", "apple"};
    int input_count = sizeof(input) / sizeof(input[0]);
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    const char *output[MAX_WORDS];

    int output_count = remove_duplicate_words(input, input_count, output, MAX_WORDS);

    for (int i = 0; i < output_count; i++) {
        printf("%s\n", output[i]);
    }

    return 0;
}