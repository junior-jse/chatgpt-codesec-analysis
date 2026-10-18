#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 100

static int is_duplicate(const char *words[], int count, const char *word) {
    if (words == NULL || word == NULL) {
        return 0;
    }
    for (int i = 0; i < count; i++) {
        if (words[i] == NULL) {
            continue;
        }
        if (strncmp(words[i], word, MAX_WORD_LEN) == 0) {
            return 1;
        }
    }
    return 0;
}

int remove_duplicate_words(const char *input[], int input_count, const char *output[], int output_max) {
    if (output == NULL || input == NULL || output_max <= 0 || input_count < 0) {
        return 0;
    }

    if (input_count > output_max) {
        return 0;
    }

    int output_count = 0;
    for (int i = 0; i < input_count; i++) {
        if (input[i] == NULL) {
            continue;
        }
        if (output_count >= output_max) {
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
    /* Possible weaknesses found:
     *  Assignment 'input_count=sizeof(input)/sizeof(input[0])', assigned value is 7
     */
    int input_count = sizeof(input) / sizeof(input[0]);
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    const char *output[MAX_WORDS] = {NULL};

    /* Possible weaknesses found:
     *  Condition 'input_count<0' is always false [knownConditionTrueFalse]
     *  Condition 'input_count<0' is always false
     */
    if (input_count < 0 || input_count > MAX_WORDS) {
        fprintf(stderr, "Invalid input count\n");
        return EXIT_FAILURE;
    }

    int output_count = remove_duplicate_words(input, input_count, output, MAX_WORDS);

    for (int i = 0; i < output_count; i++) {
        if (output[i] != NULL) {
            printf("%s\n", output[i]);
        }
    }

    return EXIT_SUCCESS;
}