/* Repair history (previous round):
 * - Flawfinder strlen was removed from calculate_min_length() and now appears in main().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate_min_length(const char *str, size_t max_len) {
    size_t len = strnlen(str, max_len);
    if (len == 0) {
        return 0;
    }

    size_t total_ops = 0;
    for (size_t i = 0; i < len; ) {
        size_t j = i + 1;
        while (j < len && str[j] == str[i]) {
            j++;
        }
        total_ops++;
        i = j;
    }

    return (int)total_ops;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    const char *input = argv[1];
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t max_len = strlen(argv[0]) + strlen(argv[1]) + 10; 
    int result = calculate_min_length(input, max_len);
    printf("%d\n", result);

    return 0;
}