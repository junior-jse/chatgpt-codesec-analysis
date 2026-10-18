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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
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

/* Possible weaknesses found:
 *  to match this '{'
 */
int main(void) {
    /* Possible weaknesses found:
     *  expected expression at end of input
     *  expected expression
     *  expected '}'
     *  missing terminating " character
     *  No pair for character ("). Can't process file. File is either invalid or unicode, which is currently not supported. [syntaxError]
     *  expected ';' at end of declaration
     *  expected declaration or statement at end of input
     */
    const char *test_string = "AAABBBCCDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD