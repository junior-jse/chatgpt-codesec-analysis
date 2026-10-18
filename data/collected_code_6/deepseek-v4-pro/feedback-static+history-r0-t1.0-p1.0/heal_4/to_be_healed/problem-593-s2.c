#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_leading_zeros(char *ip) {
    if (ip == NULL) return;

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[16] = {0};
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char temp[16];
    int first = 1;
    size_t current_len = 0;

    size_t ip_len = 0;
    /* Possible weaknesses found:
     *  Array index 'ip_len' is used before limits check. [arrayIndexThenCheck]
     */
    while (ip[ip_len] != '\0' && ip_len < sizeof(temp) - 1) {
        ip_len++;
    }
    if (ip[ip_len] != '\0') {
        return;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(temp, ip, ip_len);
    temp[ip_len] = '\0';

    char *token;
    char *saveptr;
    token = strtok_r(temp, ".", &saveptr);
    while (token != NULL) {
        char *start = token;
        while (*start == '0' && *(start + 1) != '\0') {
            start++;
        }

        if (!first) {
            if (current_len < sizeof(result) - 1) {
                result[current_len] = '.';
                current_len++;
                result[current_len] = '\0';
            }
        }

        size_t token_len = 0;
        /* Possible weaknesses found:
         *  Array index 'token_len' is used before limits check. [arrayIndexThenCheck]
         */
        while (start[token_len] != '\0' && token_len < sizeof(result) - current_len - 1) {
            token_len++;
        }
        if (current_len + token_len < sizeof(result)) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + current_len, start, token_len);
            current_len += token_len;
            result[current_len] = '\0';
        }

        first = 0;
        token = strtok_r(NULL, ".", &saveptr);
    }

    size_t result_len = 0;
    /* Possible weaknesses found:
     *  Array index 'result_len' is used before limits check. [arrayIndexThenCheck]
     */
    while (result[result_len] != '\0' && result_len < 15) {
        result_len++;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(ip, result, result_len);
    ip[result_len] = '\0';
}

int main(void) {
    char ip1[] = "192.168.001.001";
    remove_leading_zeros(ip1);
    printf("%s\n", ip1);

    char ip2[] = "010.000.000.010";
    remove_leading_zeros(ip2);
    printf("%s\n", ip2);

    char ip3[] = "0.0.0.0";
    remove_leading_zeros(ip3);
    printf("%s\n", ip3);

    char ip4[] = "255.255.255.255";
    remove_leading_zeros(ip4);
    printf("%s\n", ip4);

    return 0;
}