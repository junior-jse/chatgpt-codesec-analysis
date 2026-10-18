#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_leading_zeros(char *ip) {
    if (ip == NULL) return;

    char result[16] = {0};
    char *token;
    char *saveptr;
    char temp[16];
    int first = 1;
    size_t current_len = 0;

    strncpy(temp, ip, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

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

        size_t token_len = strlen(start);
        if (current_len + token_len < sizeof(result)) {
            strncat(result, start, sizeof(result) - current_len - 1);
            current_len += token_len;
        }

        first = 0;
        token = strtok_r(NULL, ".", &saveptr);
    }

    strncpy(ip, result, 15);
    ip[15] = '\0';
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