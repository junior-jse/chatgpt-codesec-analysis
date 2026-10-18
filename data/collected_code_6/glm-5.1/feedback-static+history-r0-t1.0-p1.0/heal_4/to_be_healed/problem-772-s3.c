#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_words_of_length_k(char *str, size_t k, size_t buffer_size) {
    if (str == NULL || k == 0 || buffer_size == 0) {
        return;
    }

    char *read = str;
    char *write = str;
    char *buf_end = str + buffer_size - 1;

    /* Possible weaknesses found:
     * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while (read <= buf_end && *read != '\0' && write <= buf_end) {
        /* Possible weaknesses found:
         * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while (read <= buf_end && *read != '\0' && isspace((unsigned char)*read)) {
            read++;
        }

        /* Possible weaknesses found:
         * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        if (read > buf_end || *read == '\0') {
            break;
        }

        /* Possible weaknesses found:
         * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        char *word_start = read;

        /* Possible weaknesses found:
         * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while (read <= buf_end && *read != '\0' && !isspace((unsigned char)*read)) {
            read++;
        }

        size_t word_len = (size_t)(read - word_start);

        if (word_len != k) {
            if (write != str) {
                if (write < buf_end) {
                    *write = ' ';
                    write++;
                } else {
                    break;
                }
            }
            size_t copy_len = word_len;
            if (write + copy_len > buf_end) {
                copy_len = (size_t)(buf_end - write);
            }
            memmove(write, word_start, copy_len);
            write += copy_len;
        }
    }

    if (write <= buf_end) {
        *write = '\0';
    } else {
        *buf_end = '\0';
    }
}

int main() {
    char str[] = "This is a test string for removing words";
    size_t k = 4;
    size_t buffer_size = sizeof(str);

    printf("Original: \"%s\"\n", str);
    remove_words_of_length_k(str, k, buffer_size);
    printf("Modified: \"%s\"\n", str);

    return 0;
}