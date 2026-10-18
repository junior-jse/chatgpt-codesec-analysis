#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void remove_leading_zeroes(char *ip) {
    if (ip == NULL || *ip == '\0') {
        return;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[16];
    char *token;
    char *saveptr;
    char *copy = strdup(ip);

    if (copy == NULL) {
        return;
    }

    token = strtok_r(copy, ".", &saveptr);
    int first = 1;
    size_t len = 0;

    while (token != NULL) {
        char *start = token;
        while (*start == '0' && *(start + 1) != '\0') {
            start++;
        }

        if (first) {
            int written = snprintf(result, sizeof(result), "%s", start);
            if (written < 0 || (size_t)written >= sizeof(result)) {
                free(copy);
                return;
            }
            len = written;
            first = 0;
        } else {
            int written = snprintf(result + len, sizeof(result) - len, ".%s", start);
            if (written < 0 || (size_t)written >= sizeof(result) - len) {
                free(copy);
                return;
            }
            len += written;
        }

        token = strtok_r(NULL, ".", &saveptr);
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(ip, result, len + 1);
    free(copy);
}

int main(void) {
    char ip1[] = "192.168.001.001";
    char ip2[] = "010.000.000.001";
    char ip3[] = "000.000.000.000";
    char ip4[] = "255.255.255.255";

    remove_leading_zeroes(ip1);
    remove_leading_zeroes(ip2);
    remove_leading_zeroes(ip3);
    remove_leading_zeroes(ip4);

    printf("%s\n", ip1);
    printf("%s\n", ip2);
    printf("%s\n", ip3);
    printf("%s\n", ip4);

    return 0;
}